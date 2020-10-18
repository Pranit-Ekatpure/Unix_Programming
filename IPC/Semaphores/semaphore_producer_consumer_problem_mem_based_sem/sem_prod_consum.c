/******************************************************************************
* Filename              : sem_prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contains example of Semaphore solution to 
*                         producer-consumer problem. This example uses memory-
*                         based Semaphore.
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
#define NBUFF 10

/******************************************************************************
* Variable Definitions
*******************************************************************************/
/* read-only by producer and consumer */
int nitems;         
/* Data shared by producer and consumer */
struct 
{
    int buff[NBUFF];
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
* Description   : main function for producer-cosumer problem using memory-based 
*                 Semaphore.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    pthread_t tid_produce, tid_consume;
    nitems = 1000000;

    /* Initialize three semaphores */
    if(sem_init(&shared.mutex, 0, 1) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init mutex\n");
        return -1;
    }
    if(sem_init(&shared.nempty, 0, NBUFF) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nempty\n");
        return -1;
    }
    if(sem_init(&shared.nstored, 0, 0) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nstored\n");
        return -1;
    }

    /* Create threads: producer and consumer */
    if(pthread_create(&tid_produce, NULL, produce, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create producer thread\n");
        return -1;
    }
    if(pthread_create(&tid_consume, NULL, consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create consumer thread\n");
        return -1;
    }

    /* Wait for threads termination */
    if(pthread_join(tid_produce, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }
    if(pthread_join(tid_consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }

    /* Remove semaphores */
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
    int i;
    for(i = 0; i < nitems; i++)
    {
        /* wait for at least 1 empty slot */
        if(sem_wait(&shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait nempty\n");
            return NULL;
        }
        if(sem_wait(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait mutex\n");
            return NULL;
        }
        /* store i into circular buffer */
        shared.buff[i % NBUFF] = i;         
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
    }
    return (NULL);
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
    for(i = 0; i < nitems; i++)
    {
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
        /* Verify that a item in the array is correct and print a message if an
         * error is found */
        if(shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n", i % NBUFF, shared.buff[i % NBUFF]);
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
    }
    return (NULL);
}
/******************************************************************************/