/******************************************************************************
* Filename              : sem_prod_consum.c
* Author                : Pranit Ekatpure
* Description           : This file contains example of Semaphore solution to 
*                         producer-consumer problem. This example uses multiple
*                         buffers with memory-based Semaphore.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define NBUFF       8
#define BUFFSIZE    512

/******************************************************************************
* Variable Definitions
*******************************************************************************/
/* data shared by producer and consumer */
struct
{                             
    struct
    {
        /* a buffer */
        char data[BUFFSIZE];  
        /* count of #bytes in the buffer */      
        ssize_t n;                  
    }buff[NBUFF];
    /* semaphores */
    sem_t mutex, nempty, nstored;   
}shared;

/* input file to copy to stdout */
int fd;    

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *produce(void *), *consume(void *);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for producer-cosumer problem with multiple 
*                 buffers using memory-based Semaphore.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    pthread_t tid_produce, tid_consume;

    fd = open("sem_prod_consum.c", O_RDONLY);

    /* initialize three semaphores */
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

    /* one producer thread, one consumer thread */
    /* reader thread */
    if(pthread_create(&tid_produce, NULL, produce, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create producer thread\n");
        return -1;
    }    
    /* writer thread */      
    if(pthread_create(&tid_consume, NULL, consume, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create consumer thread\n");
        return -1;
    }

    /* wait for threads */
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
    int i;
    for(i = 0; ; )
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
        /* Empty critical region as single producer thread used */
        if(sem_post(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post mutex\n");
            return NULL;
        }

        /* Read data and increment nstored semaphore */
        if((shared.buff[i].n = read(fd, shared.buff[i].data, BUFFSIZE)) < 0)
        {
            fprintf(stderr, "ERROR: file reading failed\n");
            return NULL;
        }
        if(shared.buff[i].n == 0)
        {
            /* End of file */
            /* 1 more stored item */
            sem_post(&shared.nstored);      
            return NULL;
        }
        /* circular buffer */
        if(++i >= NBUFF)
            i = 0;                          
        /* 1 more stored item */
        if(sem_post(&shared.nstored) != 0)
        {
            fprintf(stderr, "ERROR: producer, sem_post nstored\n");
            return NULL;
        }          
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
    for(i = 0; ; )
    {
        /* Wait for at least 1 stored items */
        if(sem_wait(&shared.nstored) == -1)
        {
            fprintf(stderr, "ERROR: consumer, sem_wait nstored\n");
            return NULL;
        }
        if(sem_wait(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_wait mutex\n");
            return NULL;
        }
        /* Empty critical region as single consumer thread used */
        if(sem_post(&shared.mutex) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post mutex\n");
            return NULL;
        }

        if(shared.buff[i].n == 0)
            return NULL;
        /* Write to Standard output */
        if(write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n) != shared.buff[i].n)
        {
            fprintf(stderr, "ERROR: failed to write to standard output\n");
            return NULL;
        }
        /* Circular buffer */
        if(++i >= NBUFF)
            i = 0;                          
        /* 1 more empty slot */
        if(sem_post(&shared.nempty) == -1)
        {
            fprintf(stderr, "ERROR: producer, sem_post nempty\n");
            return NULL;
        }
    }
}
/******************************************************************************/