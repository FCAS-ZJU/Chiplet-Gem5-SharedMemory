/* compile with gcc at optimization level -O3 */
/* set the minimum and maximum CPU frequency for all cores using cpupower to get meaningful results */ 
/* run using "sudo nice -n -20 ./a.out" to minimize possible context switches, or at least use "taskset -c 0 ./a.out" */
/* you can optionally use a p-state scaling driver other than intel_pstate to get perfectly reproducable results */

#include <string>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <stdint.h>
#include <x86intrin.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>



using namespace std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t lock;

int A[ 100 ];
int B;
string C;
uint64_t D[100];

string msg = "0101";


void write0(){
    for(int i = 0; i<2;i++){
        A[0]=1; 
        A[16]=1;
        A[32]=1;
        A[48]=1;
        _mm_mfence();
    }
    //printf("address = %p \n",&A[0]);

}

void *makenoise(void *arg){
	for(int j=0;j<msg.length();j++){
            //usleep(200);
            B = 1;
            //usleep(999800);
	}
}

void *write(void *arg){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0,&mask);
	if(pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&mask)<0){
		perror("sched_setaffinity");
	}

	for(int j=0;j<msg.length();j++){
		/*if(pthread_mutex_lock(&mutex)!=0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}*/
		//pthread_rwlock_wrlock(&lock);
		if(msg[j]=='0') 
			write0();

		printf("thread %u send %c\n",(unsigned int)(getpid()),msg[j]);
		/*if(pthread_mutex_unlock(&mutex)!=0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}*/
  		//pthread_rwlock_unlock(&lock);
                C[j]=msg[j];
		usleep(100000);
 

	}
}

void *read(void *arg){
	uint64_t time1, time2, msl, hsl, osl;
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(1,&mask);
	if(pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&mask)<0){
		perror("sched_setaffinity");
	}


        for(int j=0;j<msg.length();j++){
	    usleep(400);
	    //pthread_rwlock_rdlock(&lock);
	    /*if(pthread_mutex_lock(&mutex)!=0){
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	    }*/
	    _mm_mfence();                      /* this properly orders both clflush and rdtscp*/
	    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtscp */
	    time1 = __rdtsc();                 /* set timer */
	    _mm_lfence();                      /* serialize __rdtscp with respect to trailing instructions + compiler barrier for rdtscp and the load */
	    int temp = A[ 0 ];             /* array[0] is a cache miss */
            temp = A[16];
	    temp = A[32];
            temp = A[48];
	    /* measring the write miss latency to array is not meaningful because it's an implementation detail and the next write may also miss */
	    /* no need for mfence because there are no stores in between */
	    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtscp and the load*/
	    time2 = __rdtsc();
	    _mm_lfence();                      /* serialize __rdtscp with respect to trailing instructions */
	    hsl = time2 - time1;
	    
	    printf( "array[ 0 ] = %i \n", temp ); 
	    
	    _mm_mfence();                      /* this properly orders both clflush and rdtscp*/
	    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtscp */
	    time1 = __rdtsc();                 /* set timer */
	    _mm_lfence();                      /* serialize __rdtscp with respect to trailing instructions + compiler barrier for rdtscp */
	    /* no need for mfence because there are no stores in between */
	    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtscp */
	    time2 = __rdtsc();
	    _mm_lfence();                      /* serialize __rdtscp with respect to trailing instructions */
	    osl = time2 - time1;
	    printf( "%lu,", hsl - osl );
	    printf( "thread %u Measured latency = %lu TSC cycles\n\n",(unsigned int)(pthread_self()), hsl - osl ); /* hsl is always larger than osl */
            
            D[j] = hsl - osl;
            
	    //in<<hsl-osl<<"\n";

            _mm_clflush( &A[ 0 ] );
            _mm_clflush( &A[ 16 ] );
            _mm_clflush( &A[ 32 ] );
            _mm_clflush( &A[ 48 ] );
	    /*if(pthread_mutex_unlock(&mutex)!=0){
		perror("pthread_mutex_unlock");
		exit(EXIT_FAILURE);
	    }*/
	    //pthread_rwlock_unlock(&lock);
	    usleep(99600);
	}

}

int main()
{   

    pthread_t sender,receiver,noise;
    pthread_create(&sender,NULL,write,NULL);
    pthread_create(&receiver,NULL,read,NULL);
    //pthread_create(&noise,NULL,makenoise,NULL);
    pthread_join(sender,NULL);
    pthread_join(receiver,NULL);
    //pthread_join(noise,NULL);
    //in.close();
    printf("Hello\n");
    for(int i = 0; i<msg.length();i++){
        printf("Send %c\n", C[i]);
        printf("Delay %lu\n", D[i]);
    }
    return 0;

}
