#include<iostream>
#include<thread>
#include<string>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h>
#include <stdio.h>

using namespace std;

int A[100];
string msg = "0101";

//输出0
void write0(){
    for(int i = 0; i<2;i++){
        A[0]=1; 
        A[16]=1;
        A[32]=1;
        A[48]=1;
        //对齐fs模式 disk
        _mm_mfence();
    }
    //printf("address = %p \n",&A[0]);
}


//write_function
void write_function(){
    //cpu_set_t mask;
    //CPU_ZERO(&mask);
    //CPU_SET(0,&mask);
        //if(pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&mask)<0){
            //perror("sched_setaffinity");
	//}
    for(int j=0;j<msg.length();j++){
        if(msg[j]=='0')
	    write0();
        printf("send %c\n",msg[j]);
	usleep(100000);
    }

}


//read_function
void read_function(){
	uint64_t time1, time2, msl, hsl, osl;
	//cpu_set_t mask;
	//CPU_ZERO(&mask);
	//CPU_SET(1,&mask);
	//if(pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&mask)<0){
		//perror("sched_setaffinity");
	//}


        for(int j=0;j<msg.length();j++){
	    usleep(400);

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
	    printf( "Measured latency = %lu TSC cycles\n\n", hsl - osl ); /* hsl is always larger than osl */
            
            _mm_clflush( &A[ 0 ] );
            _mm_clflush( &A[ 16 ] );
            _mm_clflush( &A[ 32 ] );
            _mm_clflush( &A[ 48 ] );
	    usleep(99600);
	}  

}

int main(int argc, char *argv[]){
    thread **threads = new thread*[2];
    threads[0] = new thread(write_function);   
    threads[1] = new thread(read_function);
    
    threads[0]->join();
    threads[1]->join();

    delete[] threads;



    cout << "Validating..." <<endl;

    return 0;

}
