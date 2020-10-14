/******************************************************************************
* Filename              : prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contain example to show synchronization
*                         between the multiple producer threads with condition
*                         condtion variable used to wait and notify the 
*                         consumer when producer writes data to buffer.
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
#define MAXNITEMS        1000000
#define MAXNTHREADS      100

/******************************************************************************
* Variable Definitions
*******************************************************************************/
int nitems;            
int buff[MAXNITEMS];

/* Structure for producer varaibles and mutex used by producers*/
struct
{
    pthread_mutex_t mutex;
    /* next index to store */
    int nput;      
    /* next value to store */             
    int nval;                   
} put = {PTHREAD_MUTEX_INITIALIZER};

/* Structure for condition varaible, mutex and count */
struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    /* number ready for consumer */
    int nready;                 
}nready = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

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

    /* start the consumer thread */
    if(pthread_create(&tid_consume, NULL, consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create consumer thread\n");
        return -1;
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

    /* wait for the consumer thread */
    if(pthread_join(tid_consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }
    exit(0);
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
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
        if(pthread_mutex_lock(&put.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_lock\n");
            return NULL;
        }
        if(put.nput >= nitems)
        {
            /* unlock region protected with mutex lock */
            if(pthread_mutex_unlock(&put.mutex) != 0)
            {
                fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            }
            /* array is full, we're done */
            return NULL;          
        }

        /* Update the buffer, index and value */
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;

        /* unlock region protected with mutex lock */
        if(pthread_mutex_unlock(&put.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            return NULL;
        }

        /* protect region with mutex lock */
        if(pthread_mutex_lock(&nready.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_lock\n");
            return NULL;
        }
        /* Wake up the consumer who was waiting for the count nready to become 
         * nonzero */
        if(nready.nready == 0)
            if(pthread_cond_signal(&nready.cond) != 0)
            {
                fprintf(stderr, "ERROR: pthread_cond_signal\n");
                return NULL;
            }
        /* Increment the count nready: the number of items ready for the consumer
         * to process */
        nready.nready++;

        /* unlock region protected with mutex lock */
        if(pthread_mutex_unlock(&nready.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            return NULL;
        }
        
        /* Increment the count of the thread */
        *((int*)arg) += 1;
    }
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
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
    for(i = 0; i < nitems; i++)
    {
        /* protect region with mutex lock */
        if(pthread_mutex_lock(&nready.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_lock\n");
            return NULL;
        }

        /* Wait until count value becomes non zero */
        while(nready.nready == 0)
            if(pthread_cond_wait(&nready.cond, &nready.mutex) != 0)
            {
                fprintf(stderr, "ERROR: failed pthread_cond_wait\n");
                return NULL;
            }
        /* Decrement the counter */
        nready.nready--;

        /* unlock region protected with mutex lock */
        if(pthread_mutex_unlock(&nready.mutex) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_mutex_unlock\n");
            return NULL;
        }
        
        /* Verify that item in the array is correct and print a message if an
         * erro is found */
        if(buff[i] != i)
            printf("buff[%d] = %d\n", i, buff[i]);
    }
    return (NULL);
}
/******************************************************************************/