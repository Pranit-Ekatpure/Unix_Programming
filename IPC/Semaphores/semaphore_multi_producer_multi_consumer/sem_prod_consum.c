/******************************************************************************
* Filename              : sem_prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contains example of Semaphore solution to 
*                         producer-consumer problem. This example uses multiple
*                         producer, multiple consumer with memory-based Semaphore.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define NBUFF       10
#define MAXNTHREADS 100

/******************************************************************************
* Variable Definitions
*******************************************************************************/
/* Read-only by producer and consumer */
int nitems, nproducers, nconsumers;       
/* data shared by producer and consumer */
struct                                      
{
    int buff[NBUFF];  
    /* item number: 0, 1, 2,.... */          
    int nput;         
    /* value to store in buff[] */                      
    int nputval;  
    /* item number: 0, 1, 2,.... */                          
    int nget;       
    /* value to store in buff[] */                        
    int ngetval;   
    /* semaphores */                         
    sem_t mutex, nempty, nstored;           
}shared;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *produce(void *), *consume(void *);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for producer-cosumer problem, multiple producers
*                 multiple consumer, using memory-based Semaphore.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int i, prodcount[MAXNTHREADS], conscount[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume[MAXNTHREADS];

    nproducers = 10;
    nconsumers = 10;
    nitems = 1000000;

    /* Initialize three semaphores */
    if(sem_init(&shared.mutex, 0, 1) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init mutex\n");
        return -1;
    }
    if(sem_init(&shared.nempty, 0 , NBUFF) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nempty\n");
        return -1;
    }
    if(sem_init(&shared.nstored, 0, 0) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nstored\n");
        return -1;
    }

    /* Create all producers and all consumers */
    for(i = 0; i < nproducers; i++)
    {
        prodcount[i] = 0;
        if(pthread_create(&tid_produce[i], NULL, produce, &prodcount[i]) != 0)
        {
            fprintf(stderr, "ERROR: failed to create producer thread\n");
            return -1;
        }
    }
    for(i = 0; i < nconsumers; i++)
    {
        conscount[i] = 0;
        if(pthread_create(&tid_consume[i], NULL, consume, &conscount[i]) != 0)
        {
            fprintf(stderr, "ERROR: failed to create consumer thread\n");
            return -1;
        }
    }

    /* Wait for producers and consumers */
    for(i = 0; i < nproducers; i++)
    {
        if(pthread_join(tid_produce[i], NULL) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_join\n");
            return -1;
        }
        /* print how many time each producer thread run */
        printf("producer count[%d] = %d\n", i, prodcount[i]);
    }
    printf("\n");
    for(i = 0; i < nconsumers; i++)
    {
        if(pthread_join(tid_consume[i], NULL) != 0)
        {
            fprintf(stderr, "ERROR: failed pthread_join\n");
            return -1;
        }
        /* print how many time each consumer thread run */
        printf("consumer count[%d] = %d\n", i, conscount[i]);
    }

    /* Delete semaphores */
    if(sem_destroy(&shared.mutex) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_destroy mutex\n");
        return -1;
    }
    if(sem_destroy(&shared.nempty) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_destroy nempty\n");
        return -1;
    }
    if(sem_destroy(&shared.nstored) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_destroy nstored\n");
        return -1;
    }

    exit(0);
}

/******************************************************************************
* Function      : produce
* Description   : producer function.
*
* Parameters    : 
*   void *arg   : void pointer
* Return value  : void pointer 
*
*******************************************************************************/
void *produce(void *arg)
{
    for( ; ; )
    {
        /* wait for at least 1 empty slot */
        if(sem_wait(&shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait nempty\n");
            return NULL;
        }  

        /* Multiple producer threads can acquire the nempty semaphore at the same
         * time, but only one producer thread at a time can acquire the mutex
         * semaphore. This protects the shared variables from being modified by
         * more than one producer thread at a time. */         
        if(sem_wait(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait mutex\n");
            return NULL;
        }
        
        if(shared.nput >= nitems)
        {
            /* let consumers terminate */
            if(sem_post(&shared.nstored) != 0)
            {
                fprintf(stderr, "ERROR: producer, sem_post nstored\n");
                return NULL;
            }
            if(sem_post(&shared.nempty) == -1)
            {
                fprintf(stderr, "ERROR: producer, sem_post nempty\n");
                return NULL;
            }
            if(sem_post(&shared.mutex) == -1)
            {
                fprintf(stderr, "ERROR: producer, sem_post mutex\n");
                return NULL;
            }      
            return NULL;                  /* all done */
        }

        /* Update the shared data */
        shared.buff[shared.nput % NBUFF] = shared.nputval;
        shared.nput++;
        shared.nputval++;

        if(sem_post(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post mutex\n");
            return NULL;
        }
        /* 1 more stored item */
        if(sem_post(&shared.nstored) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post nstored\n");
            return NULL;
        }  
        /* Increment the count of the producer thread */        
        *((int *)arg) += 1;
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
    for( ; ; )
    {
        int i;

        /* wait for at least 1 stored items */
        if(sem_wait(&shared.nstored) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_wait nstored\n");
            return NULL;
        }
        if(sem_wait(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_wait mutex\n");
            return NULL;
        }

        if(shared.nget >= nitems)
        {
            if(sem_post(&shared.nstored) != 0)
            {
                fprintf(stderr, "ERROR: producer, sem_post nstored\n");
                return NULL;
            }
            if(sem_post(&shared.mutex) == -1)
            {
                fprintf(stderr, "ERROR: producer, sem_post mutex\n");
                return NULL;
            }
            return (NULL);                  /* all done */
        }

        /* Verify that an item in the array is correct and print a message if an
         * error is found */
        i = shared.nget % NBUFF;
        if(shared.buff[i] != shared.ngetval)
            printf("error: buff[%d] = %d\n", i, shared.buff[i]);
        shared.nget++;
        shared.ngetval++;

        if(sem_post(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_post mutex\n");
            return NULL;
        }
        /* 1 more empty slot */
        if(sem_post(&shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_post nempty\n");
            return NULL;
        }

        /* Increment the count of the consumer thread */
        *((int *) arg) += 1;
    }
}
/******************************************************************************/
