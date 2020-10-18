/******************************************************************************
* Filename              : simple_thread.c
* Author                : Pranit Ekatpure
* Description           : This file contain simple thread creation example.
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
pthread_t ntid;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void printids(const char *s);
void *thr_fn(void *arg);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function that creates one thread and prints the process 
*                 and thread IDs of the new thread and the initial thread.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int err;

    if((err = pthread_create(&ntid, NULL, thr_fn, NULL)) != 0)
    {
        fprintf(stderr,"ERROR: can't create thread\n");
        return -1;
    }
    printids("Main thread:");
    sleep(1);
    exit(0);
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : thr_fn
* Description   : Thread function
*
* Parameters    : void pointer
* Return value  : void pointer
*
*******************************************************************************/
void *thr_fn(void *arg)
{
    printids("New thread: ");
    return ((void*)0);
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : printids
* Description   : Print function for printing IDs of process and thread.
*
* Parameters    : 
*   const char *s : Pointer to character string
* Return value  : void pointer
*
*******************************************************************************/
void printids(const char *s)
{
    pid_t       pid;
    pthread_t   tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
            (unsigned long)tid, (unsigned long)tid);
}
/******************************************************************************/

