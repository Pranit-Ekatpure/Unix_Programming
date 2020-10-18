/******************************************************************************
* Filename              : thread_cancellation.c
* Author                : Pranit Ekatpure
* Description           : This file contain test program to show thread 
*                         cancellation and read write lock.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
* Variable Definitions
*******************************************************************************/
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_t tid1, tid2;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *thread1(void *), *thread2(void *);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main fuction for program to show thread cancellation and read 
*                 write lock.
*
* Parameters    :
* Return value  :
*
*******************************************************************************/
int main(void)
{
    void *status;
    /* Create two threads */
    if(pthread_create(&tid1, NULL, thread1, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create thread1\n");
        return -1;
    }
    /* Let thread1() get the lock */
    sleep(1);   
    if(pthread_create(&tid2, NULL, thread2, NULL) != 0)
    {
        fprintf(stderr, "ERROR: failed to create thread2\n");
        return -1;
    }
    /* Wait for thread termination */
    if(pthread_join(tid2, &status) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }
    if(status != PTHREAD_CANCELED)
        printf("Thread2 status = %p\n", status);
    if(pthread_join(tid1, &status) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_join\n");
        return -1;
    }
    if(status != NULL)
        printf("Thread1 status = %p\n", status);
    /* Destriy read-write lock object */
    if(pthread_rwlock_destroy(&rwlock) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_rwlock_destroy rwlock");
        return -1;
    }

    exit(0);
}

/******************************************************************************
* Function      : thread1
* Description   : Thread function 1
*
* Parameters    : void pointer
* Return value  : void pointer
*
*******************************************************************************/
void *thread1(void *arg)
{
    if(pthread_rwlock_rdlock(&rwlock) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_rwlock_rdlock\n");
        return NULL;
    }
    printf("Thread1() got a read lock\n");
    /* let thread2 block in pthread_rwlock_wrlock() */
    sleep(3); 
    /* Cancel thread 2 */                  
    if(pthread_cancel(tid2) != 0)
    {
        fprintf(stderr, "ERROR: pthread_cancel\n");
        return NULL;
    }
    sleep(3);
    if(pthread_rwlock_unlock(&rwlock) != 0)
    {
        fprintf(stderr, "ERROR: pthread_rwlock_unlock\n");
        return NULL;
    }
    return (NULL);
}

/******************************************************************************
* Function      : thread1
* Description   : Thread function 1
*
* Parameters    : void pointer
* Return value  : void pointer
*
*******************************************************************************/
void *thread2(void *arg)
{
    printf("Thread2() trying to obtain a write lock\n");
    if(pthread_rwlock_wrlock(&rwlock) != 0)
    {
        fprintf(stderr, "ERROR: failed pthread_rwlock_wrlock\n");
        return NULL;
    }
    sleep(3);
    /* Should not get here */
    printf("Thread2() got a write lock\n");         
    sleep(1);
    if(pthread_rwlock_unlock(&rwlock) != 0)
    {
        fprintf(stderr, "ERROR: pthread_rwlock_unlock\n");
        return NULL;
    }
    return (NULL);
}
/******************************************************************************/