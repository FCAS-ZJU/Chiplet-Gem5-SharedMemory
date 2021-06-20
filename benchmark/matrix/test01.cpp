#include <pthread.h>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <cstring>
#include <assert.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "./../gem5/m5ops.h"
using namespace std;

int* matrix1;
int* matrix2;
int* matrix3;

int N = 300;
int theChipletScale;
const int thread_num = 4;
int coreNum;
extern "C"{
    void init(int id){
        // initialize matrix
        coreNum = sysconf(_SC_NPROCESSORS_ONLN);
        

        matrix1 = (int*)malloc(N*N*sizeof(int));
        matrix2 = (int*)malloc(N*N*sizeof(int));
        matrix3 = (int*)malloc(N*N*sizeof(int));
        
        for(int i = 0;i < N*N;i++){

            matrix1[i] = 1;
            matrix2[i] = 2;

        }
        memset(matrix3, 0, N*N*sizeof(int));  

        // initialize the info about the gem5 
        
        m5_gadia_call(id, 1, 0, 0);


    }
}

struct Args{
    

    int offset;
    int length;
    pthread_barrier_t *barrier;
    // Variables needed for timing
    int num_threads;
    int *counter;
    pthread_mutex_t *mtx;
    pthread_cond_t *cond;
    timeb *start;
    timeb *end;
    int processID;
};


void perf_cycle(int num_threads, int *counter, pthread_mutex_t *mtx,
        pthread_cond_t *cond,
        timeb *time, int id){
    // Get the lock
    pthread_mutex_lock(mtx);

    // Atomically decrement number of outstanding threads
    *counter -= 1;
    // Check if we are the last thread
    // If not, wait to be signaled
    if(*counter == 0){
        // Update a timing variable
        
        if(id == 0){
            //m5_gadia_call(id, 0, 0, 2); 
        }else{
	    m5_gadia_call(id, 0, 0, 2);
        uint64_t a;
        while(1){
            a = m5_gadia_receive(theChipletScale);
            if(a == (uint64_t)-4){
              break;
            }
          }
        }

        ftime(time);

        // Reset the counter
        *counter = num_threads;

        // Signal everyone to continue
        pthread_cond_broadcast(cond);
    }else{
        // Wait for the last thread before continuing
        pthread_cond_wait(cond, mtx);
    }

    // Everyone unlocks
    pthread_mutex_unlock(mtx);
}



void caculate(int offset, int length){


      for(int i = 0;i< length;i++){
        int current_x = (i + offset) / N;
        int current_y = (i + offset) % N;
        for(int k = 0;k < N;k++)
          matrix3[i + offset] = matrix3[i + offset] + matrix1[current_y*N + k] * matrix2[k * N + current_x];

      }


 

}

void toWrite(int offset, int length, pthread_mutex_t *mtx){

      pthread_mutex_lock(mtx);
        for(int i = 0;i< length;i++){
            m5_gadia_call(0, 0, matrix3[i + offset], 0);
        }
      pthread_mutex_unlock(mtx);


}



int core_counter = 0;
// Pthread function for computing
// Takes a pointer to a struct of args as an argument
void *matrix_parallel(void *args){
    // Cast void pointer to struct pointer
    Args *local_args = (Args*)args;
    
    cpu_set_t mask;
    CPU_ZERO(&mask);

    // Unpack the arguments
    pthread_barrier_t *barrier = local_args->barrier;


    int length = local_args->length;
    int offset = local_args->offset;
    int num_threads = local_args->num_threads;
    int *counter = local_args->counter;
    pthread_mutex_t *mtx = local_args->mtx;
    pthread_cond_t *cond = local_args->cond;
    timeb *start = local_args->start;
    timeb *end = local_args->end;
    int id = local_args->processID;




    pthread_mutex_lock(mtx);

    if(core_counter >= coreNum){
        core_counter = 0;
    }
    CPU_SET(core_counter, &mask);
    int a = sched_setaffinity(0, sizeof(mask), &mask);
    std::cout <<"counter is: " << core_counter << " and affinity is: " << a << std::endl;
    core_counter++;

    pthread_mutex_unlock(mtx);



    // Wait for all threads to be created before profiling
    perf_cycle(num_threads, counter, mtx, cond, start, id);



    caculate(offset, length);

    toWrite(offset, length, mtx);

    // Stop monitoring when last thread exits
    perf_cycle(num_threads, counter, mtx, cond, end, id);
    
    // Write the data
    //toWrite(offset, length, mtx);

    return 0;
}

// Helper function create thread 
void launch_threads(int num_threads, int id){

    // Create array of thread objects we will launch
    pthread_t threads[num_threads];

    // Create a barrier and initialize it
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Create an array of structs to pass to the threads
    Args thread_args[num_threads];

    // Create variables for performance monitoring
    int counter = num_threads;
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    timeb start;
    timeb end;

    m5_gadia_call(0, 1, 0, -1);
    while(1){
    	uint64_t a = m5_gadia_receive(theChipletScale);
	if(a == (uint64_t)-4)
		break;
    }

    // Launch threads
    for(int i = 0; i < num_threads; i++){
        // Pack struct with its arguments
        thread_args[i].barrier = &barrier;
        thread_args[i].offset = i*N*N/thread_num/2 + id*N*N/2;
        thread_args[i].length = N*N/thread_num/2;
        thread_args[i].num_threads = num_threads;
        thread_args[i].counter = &counter;
        thread_args[i].mtx = &mtx;
        thread_args[i].cond = &cond;
        thread_args[i].start = &start;
        thread_args[i].end = &end;
        thread_args[i].processID = id;
        // Launch the thread
        pthread_create(&threads[i], NULL, matrix_parallel, (void*)&thread_args[i]);
    }

    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }


    long long timeCost = (long long)end.time*1000+end.millitm - (long long)start.time*1000+start.millitm;
    // Print out the time
    cout << " time parallel = " << timeCost << " millionseconds" <<  endl;
}

void print(){

    for(int i = 0;i < N*N;i++)
        std::cout << matrix3[i] << " ";

}


// 300*300
// A chiplet scale is sql(N*N/chipletNum)

int main(){

    int id;
    std::cout << "enter the id" << std::endl;
    std::cin >> id;
    std::cout << "enter chiplet number" << std::endl;
    std::cin >> theChipletScale;
    std::cout << " the scale N is: " << std::endl;
    std::cin >> N;
    //N = (int)sqrt(300*300/theChipletScale);
    init(id);
    launch_threads(thread_num, id);
    std::cout << "current thread_num is: " << thread_num << std::endl;
    std::cout << "scale is: " << N << std::endl;

    return 0;
}
