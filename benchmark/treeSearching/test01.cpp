// C++ implementation of the approach
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <algorithm>
#include "./../gem5/m5ops.h"
const int thread_Num = 3;
const int totalFileLine = 27807;
int totalVertex = 7624;
pthread_mutex_t *mtx;
pthread_cond_t *cond;
pthread_mutex_t *data_mtx;
pthread_t threads[thread_Num + 1];
pthread_barrier_t _barrier;
class Node;
int *totalNode;
int *resultNode;
int *processNode;
int *bfsQueue;
int bfsQueue_length= 0;
int bfsQueue_counter = 0;
bool *visited;
bool mainThread;
bool subThread;
bool SomebodyWaiting;
int coreNum;
int *thread_counter;
int initValue;
int theChipletScale;
int processID;
using namespace std;
timeb myStart;
timeb myEnd;

// every node has a number

struct myArgs
{
    pthread_barrier_t *_barrier;
    int thread_id;
};

//  it indicates node number1 has an undirected edge
// to node number2
void connectNode(int number1, int number2, int *num)
{

    // from number1 -> number2
    int postion1 = number1 * totalVertex + number2;

    // from number2 -> number1
    int position2 = number2 * totalVertex + number1;

    num[position2] = 1;
    num[postion1] = 1;
}

void initVetex()
{

    totalNode = (int *)malloc(totalVertex * totalVertex * sizeof(int));
    memset(totalNode, 0, totalVertex * totalVertex * sizeof(int));

    resultNode = (int *)malloc(totalVertex * totalVertex * sizeof(int));
    memset(resultNode, 0, totalVertex * totalVertex * sizeof(int));
}

void initGraph()
{

    initVetex();

    // build the graph
    FILE *file = fopen("./data.csv", "r");
    fseek(file, 13L, 0);
    for (int i = 0; i < totalFileLine; i++)
    {

        int a;
        int b;
        if (fscanf(file, "%d,%d", &a, &b) == EOF)
            break;
        connectNode(a, b, totalNode);
    }

    fclose(file);
}

int unsafeGetData(bool isMain)
{


    int tmp = -1;
    if(!isMain && bfsQueue_length - bfsQueue_counter <= 2)
        return tmp;

    if (bfsQueue[bfsQueue_counter] == -1 || bfsQueue_counter == totalVertex)
        return tmp;
    
    pthread_mutex_lock(data_mtx);
    
    if (bfsQueue_counter == bfsQueue_length)
    {
        pthread_mutex_unlock(data_mtx);
        return tmp;
    }
    tmp = bfsQueue[bfsQueue_counter];
    bfsQueue_counter += 1;
    pthread_mutex_unlock(data_mtx);


    if ( isMain &&bfsQueue_length - bfsQueue_counter >= 5)
    {
        //pthread_mutex_lock(mtx);
        //std::cout << k++ << std::endl;
        if (*thread_counter != thread_Num)
        {
            //std::cout << "let it go !" << std::endl;
            pthread_cond_broadcast(::cond);
            //SomebodyWaiting = false;
        }
        //pthread_mutex_unlock(mtx);
    }
    return tmp;
}

void init()
{


    //std::cout << "enter id" << std::endl;
    //std::cin >> processID;
    /*
    if(processID == 0)
        initValue = 0;
    else
        initValue = 6666;
	*/

    initGraph();
    //threadLib = (bool*)malloc((thread_Num + 1)*sizeof(bool));
    visited = (bool *)malloc(totalVertex * sizeof(bool));
    memset(visited, 0, totalVertex * sizeof(bool));
    //memset(threadLib, 0, (thread_Num + 1)*sizeof(bool));
    mainThread = false;
    subThread = false;
    SomebodyWaiting = false;
    coreNum = sysconf(_SC_NPROCESSORS_ONLN);
}

void printGraph()
{

    for (int i = 0; i < totalVertex; i++)
    {

        std::cout << i << "-> ";
        for (int j = 0; j < totalVertex; j++)
        {

            if (totalNode[i * totalVertex + j] == 1)
                std::cout << j << ",";
        }

        std::cout << std::endl;
    }
}

void printResult()
{

    for (int i = 0; i < totalVertex; i++)
    {

        std::cout << i << "-> ";
        for (int j = 0; j < totalVertex; j++)
        {

            if (resultNode[i * totalVertex + j] == 1)
                std::cout << j << ",";
        }

        std::cout << std::endl;
    }
}

bool whertherInLine(int id, int *myStack, int _counter)
{

    int counter = _counter;
    // from counter 2 the end of stack

    while (1)
    {

        if (counter == totalVertex)
        {
            return false;
        }

        if (myStack[counter] == -1)
            return false;

        if (myStack[counter] == id)
            return true;

        counter++;
    }
}

void processCurrentNode(int id, int *length, int *myStack)
{
    if (visited[id])
        return;
    else{

	// some collision will occur if the call is executed
        //m5_gadia_call(processID, 0, id, 0);
    }
    visited[id] = true;
    //std::cout << id << " ";
    for (int i = 0; i < totalVertex; i++)
    {
        if (totalNode[id * totalVertex + i] == 1 && visited[i] == false)
        {

            if (!whertherInLine(i, myStack, 0))
            {
                //std::cout << i << " ";
                myStack[*length] = i;
                *length += 1;
                m5_gadia_call(0, 0, i, 0);
            }
        }
    }

    //std::cout << *length << std::endl;
}

void* normalBFS_for_test(void* args)
{
    // We define the totalNode[0] as the root which we will start our work
    int previous = initValue;
    // stack initialization
    bfsQueue = (int *)malloc(totalVertex * sizeof(int));
    // -1 indicates the value is not initialized
    for (int i = 0; i < totalVertex; i++)
    {
        bfsQueue[i] = -1;
    }
    pthread_barrier_wait(&::_barrier);

    //myStack[0] = previous;
    while (1)
    {
        processCurrentNode(previous, &::bfsQueue_length, ::bfsQueue);

        if (bfsQueue[bfsQueue_counter] == -1 || bfsQueue_counter == totalVertex)
            break;
        int current = unsafeGetData(true);
    
        if(current == -1)
            break;
        connectNode(previous, current, resultNode);
        previous = current;
    }
    mainThread = true;
    std::cout << "mainThread is over" << std::endl;
    while(!subThread){
	std::cout << *thread_counter << std::endl;
	    //busy waitting
    }
    
    ftime(&::myEnd);
    //normalBFS_for_test();
    //printGraph();
    //printResult();
    long long timeCost = (long long)::myEnd.time*1000+::myEnd.millitm - (long long)::myStart.time*1000+::myStart.millitm;
    // Print out the time
    cout << "Thread Number is: " << thread_Num << std::endl;
    cout << " time parallel = " << timeCost << " millionseconds" <<  endl;

    return NULL;
}

void stackPush(int* singleStack, int* counter, int value){

    *counter += 1;
    singleStack[*counter] = value;

}


void my_DFS(int id)
{

    int* singleStack = (int*)malloc(totalVertex*sizeof(int));
    int* counter;
    int a = -1;
    counter = &a;
    // init the stack
    for(int i = 0;i < totalVertex;i++){

        singleStack[i] = -1;

    }
    stackPush(singleStack, counter, id);

    int previous = id;
    while (1)
    {
        
        //std::cout << previous << " ";
        for (int i = 0; i < totalVertex; i++)
        {
            int current_vertex = i;
            if (visited[current_vertex])
                continue;

            // first we check the node
            if (whertherInLine(current_vertex, bfsQueue, 0))
            {
                continue;
            }
            // then we let it be visited
            visited[current_vertex] = true;
            m5_gadia_call(0, 0, current_vertex, 0);
            // finally we add it to the thread_stack

            stackPush(singleStack, counter, current_vertex);
        }

        if (*counter == -1)
            break;
        int current = singleStack[*counter];
        singleStack[*counter] = -1;
        *counter -= 1;
        //thread_stack.pop();
        connectNode(previous, current, resultNode);
        previous = current;
    }
}

bool anotherChipletCmplt = false;
void* checkThread(void* args){


    while(1){
        if(mainThread && subThread)
            break;
        uint64_t result = m5_gadia_receive(-1);

        if(result == (uint64_t)-2 || result == (uint64_t)-1 ||result == (uint64_t)-4 || result == (uint64_t)-6)
            continue;

        int tmp = (int)result;
	//std::cout << tmp << std::endl;
        visited[tmp] = true;

    }

    //m5_gadia_call(processID, 0, 0, 1);

    return NULL;
}

// -1 means the function should quit
int Idon_tKnowHow2Quit()
{

    int tmp = -1;

    pthread_mutex_lock(mtx);

    tmp = unsafeGetData(false);
    if (tmp == -1)
    {
        SomebodyWaiting = true;
        //std::cout << "Before thread_counter is: " << *thread_counter << std::endl;
        *thread_counter -= 1;
    
        if (*thread_counter == 0){
            subThread = true;
            std::cout << "subThread is true" << std::endl;
        }
        //std::cout << "one to sleep" << std::endl;
        //pthread_cond_wait(cond, mtx);
        while(1){
            pthread_cond_wait(cond, mtx);
            tmp = unsafeGetData(false);
            if(tmp != -1)
                break;
        }


	subThread = false;
    	*thread_counter += 1;
    	//std::cout << " Afer thread_counter is: " << *thread_counter << std::endl;
	pthread_mutex_unlock(mtx);
	return tmp;

    }else{


        pthread_mutex_unlock(mtx);
        return tmp;
    }
    //std::cout << "one go ahead" << std::endl;

    //subThread = false;
    //*thread_counter += 1;
    //std::cout << " Afer thread_counter is: " << *thread_counter << std::endl;
    pthread_mutex_unlock(mtx);

    return tmp;
}

int core_counter = 0;
void *pthread2DFS(void *args)
{

    cpu_set_t mask;
    CPU_ZERO(&mask);
    pthread_mutex_lock(mtx);

    if (core_counter >= coreNum)
    {
        core_counter = 0;
    }
    CPU_SET(core_counter, &mask);
    int a = sched_setaffinity(0, sizeof(mask), &mask);
    std::cout << "counter is: " << core_counter << " and affinity is: " << a << std::endl;
    core_counter++;
    pthread_mutex_unlock(::mtx);

    pthread_barrier_wait(&::_barrier);
    // here we should wait the other thread enter the area

    // create a stack with a list which we can check the element
    while (1)
    {
        int tmp = Idon_tKnowHow2Quit();

        if (tmp == -1)
            continue;
        //std::cout << tmp << " " ;
        my_DFS(tmp);
    }

    //std::cout << "one thread complt" << std::endl;

    return NULL;
}


// Driver code
int main()
{
    
    std::cout << "enter the root(initValue)" << std::endl;
    std::cin >> initValue;
    std::cout << "enter chiplet number" << std::endl;
    std::cin >> theChipletScale;

    init();


    
    std::cout << "init done !" << std::endl;


    m5_gadia_call(0, 0, 0, 1);
    while(1){
        uint64_t a = m5_gadia_receive(theChipletScale);
        if(a == (uint64_t)-4)
                break;
    }

    std::cout << "start!" << std::endl;

    //initPthread
    pthread_mutex_t _mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t _cond = PTHREAD_COND_INITIALIZER;
    ::mtx = &_mtx;
    ::cond = &_cond;
    pthread_barrier_init(&::_barrier, NULL, thread_Num + 1);
    pthread_mutex_t _data_mtx = PTHREAD_MUTEX_INITIALIZER;
    ::data_mtx = &_data_mtx;
    int a = thread_Num;
    ::thread_counter = &a;
    //m5_gadia_call(processID, 1, 0, 0);
    for(int i = 0;i < thread_Num;i++){
        pthread_create(&threads[i], NULL, pthread2DFS, NULL);
    }

    pthread_t check;
    pthread_create(&check, NULL, checkThread, NULL);
    pthread_detach(check);

    pthread_create(&threads[thread_Num], NULL, normalBFS_for_test, NULL);
    ftime(&::myStart);
    ftime(&::myEnd);
    // initPthread
    // let threads go 
    for(int i = 0;i <= thread_Num;i++){
        pthread_detach(threads[i]);
    }

    
    while(!subThread || !mainThread){
        // busy waiting
    }
   
    
    ftime(&::myEnd);
    //normalBFS_for_test();
    //printGraph();
    //printResult();
    long long timeCost = (long long)::myEnd.time*1000+myEnd.millitm - (long long)::myStart.time*1000+myStart.millitm;
    // Print out the time
    cout << "Thread Number is: " << thread_Num << std::endl;
    cout << " time parallel = " << timeCost << " millionseconds" <<  endl;
}
