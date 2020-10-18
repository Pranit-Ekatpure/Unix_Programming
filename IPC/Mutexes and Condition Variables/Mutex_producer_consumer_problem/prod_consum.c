/******************************************************************************
* Filename              : prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contain example to show synchronization
*                         between the multiple producer threads.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MAXNITEMS       1000000
#define MAXNTHREADS     100

/******************************************************************************
* Variable Definitions
*******************************************************************************/
/* These variables are shared between the threads. The variables are collected 
 * into a structure along with the mutex: variables should be accessed only when
 * the mutex is held. nput is the next index to store in the buff array, and nval
 * is the next value to store (0, 1, 2, and so on). Mutex variable is statically
 * allocated with initializer constant. */
int nitems;             
struct
{
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput;
    int nval;
} shared = {PTHREAD_MUTEX_INITIALIZER};

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *produce(void*), *consume(void*);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for producer consumer example. Producer threads
*                 threads are synchronized using mutex.
*
* Parameters    : void 
* Return value  : int 
*
*******************************************************************************/
int main(void)
{
    int i, nthreads, count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    /* The number of items for the producers to store */
    nitems = MAXNITEMS;    
    /* The number of producer threads to create */     
    nthreads = 5;               

    /* Start all the producer threads, each executes the function produce */
    for(i = 0; i < nthreads; i++)
    {
        count[i] = 0;
        /* The argument to each producer thread is a pointer to an element of the 
         * count array. we first initialize the counter to 0, and each thread then 
         * increment this counter each time it stores an item in the buffer. We 
         * print this array of counters when we are done, to see how many items 
         * were stored by each producer. */
        if(pthread_create(&tid_produce[i], NULL, produce, &count[i]) != 0)
        {
            fprintf(stderr, "ERROR: failed to create producer thread\n");
            return -1;
        } 
    }

    /* wait for all the producer threads */
    for(i = 0; i < nthreads; i++)
    {
        if(pthread_join(tid_produce[i], NULL) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_join\n");
            return -1;
        }
        /* print each threads counter */
        printf("count[%d] = %d\n", i, count[i]);
    }
    /* start, then wait for the consumer thread */
    if(pthread_create(&tid_consume, NULL, consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create consumer thread\n");
        return -1;
    }
    if(pthread_join(tid_consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }
    exit(0);
}

/******************************************************************************
* Function      : produce
* Description   : producer function.
*
* Parameters    : 
*   void *arg   : void pointer, pointer to an element of count array
* Return value  : void pointer 
*
*******************************************************************************/
void *produce(void *arg)
{
    for( ; ; )
    {
        /* protect region with mutex lock */
        if(pthread_mutex_lock(&shared.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_lock\n");
            return NULL;
        }
        if(shared.nput >= nitems)
        {
            /* unlock region protected with mutex lock */
            if(pthread_mutex_unlock(&shared.mutex) != 0)
            {
                fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            }
            /* array is full, we're done */
            return NULL;          
        }
        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        /* unlock region protected with mutex lock */
        if(pthread_mutex_unlock(&shared.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            return NULL;
        }
        /* Increment the count of the thread */
        *((int*)arg) += 1;
    }
}

/******************************************************************************
* Function      : consume
* Description   : consumer function.
*
* Parameters    : 
*   void *arg   : void pointer
* Return value  : void pointer 
*
*******************************************************************************/
void *consume(void *arg)
{
    int i;
    /* Verify that each item in the array is correct and print a message if an
     * error is found */
    for(i = 0; i < nitems; i++)
    {
        if(shared.buff[i] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i]);
    }
    return (NULL);
}
/******************************************************************************/