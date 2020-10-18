/******************************************************************************
* Filename              : sem_prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contains example of Semaphore solution to 
*                         producer-consumer problem. This example uses named 
*                         Semaphore.
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

/******************************************************************************
* Macros
*******************************************************************************/
#define SEM_MUTEX   "/mutex"
#define SEM_NEMPTY  "/nempty"
#define SEM_NSTORED "/nstored"

/******************************************************************************
* Variable Definitions
*******************************************************************************/
/* Read-only by producer and consumer */
int nitems;        
/* Data shared by producer and consumer */
struct
{
    int buff[NBUFF];
    sem_t *mutex, *nempty, *nstored;
} shared;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *produce(void *), *consume(void *);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for producer-cosumer problem using named 
*                 Semaphore.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    pthread_t tid_produce, tid_consume;
    nitems = 100;

    /* Create three semaphores */
    if((shared.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to create %s\n", SEM_MUTEX);
        return -1;
    }
    if((shared.nempty = sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, 0666, NBUFF)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to create %s\n", SEM_NEMPTY);
        return -1;
    }
    if((shared.nstored = sem_open (SEM_NSTORED, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to create %s\n", SEM_NSTORED);
        return -1;
    }

    /* Create one producer thread and one consumer thread */
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

    /* Wait for the two threads */
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

    /* Remove the semaphores */
    if(sem_unlink(SEM_MUTEX) == -1)
    {
        fprintf(stderr, "ERROR: sem_unlink %s\n", SEM_MUTEX);
        return -1;
    }
    if(sem_unlink(SEM_NEMPTY) == -1)
    {
        fprintf(stderr, "ERROR: sem_unlink %s\n", SEM_NEMPTY);
        return -1;
    }
    if(sem_unlink(SEM_NSTORED) == -1)
    {
        fprintf(stderr, "ERROR: sem_unlink %s\n", SEM_NSTORED);
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
        if(sem_wait(shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait nempty\n");
            return NULL;
        }
        if(sem_wait(shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait mutex\n");
            return NULL;
        }
        /* store i into circular buffer */
        shared.buff[i % NBUFF] = i;         
        if(sem_post(shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post mutex\n");
            return NULL;
        }
        /* 1 more stored item */
        if(sem_post(shared.nstored) == -1)
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
        if(sem_wait(shared.nstored) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_wait nstored\n");
            return NULL;
        }
        if(sem_wait(shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_wait mutex\n");
            return NULL;
        }
        /* Verify that a item in the array is correct and print a message if an
         * error is found */
        if(shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n", i % NBUFF, shared.buff[i % NBUFF]);
        if(sem_post(shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_post mutex\n");
            return NULL;
        }
        /* 1 more empty slot */
        if(sem_post(shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_post nempty\n");
            return NULL;
        }      
    }
    return (NULL);
}
/******************************************************************************/