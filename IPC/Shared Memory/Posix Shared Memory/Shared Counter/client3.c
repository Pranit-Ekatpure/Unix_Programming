/******************************************************************************
* Filename              : client3.c
* Author                : Pranit Ekatpure
* Description           : This file contains Shared Counter example' client
*                         implentation.
*******************************************************************************/
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
* Macros
*******************************************************************************/
#define SEM_NAME        "/mysem"
#define SHARED_FILE     "temp"

/******************************************************************************
* Includes
*******************************************************************************/
/* Struct stored in shared memory */
struct shmstruct                
{
    int count;
};
/* pointer to named semaphore */
sem_t *mutex;                   

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for client that increments a counter in shared
*                 memory.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int fd, i, nloop;
    pid_t pid;
    struct shmstruct *ptr;    
    nloop = 100;
    
    /* Open shared object which must already exists */
    if((fd = shm_open(SHARED_FILE, O_RDWR, 0666)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open shared memory object\n");
        return -1;
    }
    /* Map memory into address space of process */
    if((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_WRITE | PROT_WRITE, 
        MAP_SHARED, fd, 0)) == MAP_FAILED)
        {
            fprintf(stderr, "ERROR: failed to mmap\n");
            return -1;
        }
    if(close(fd) == -1)
        fprintf(stderr, "ERROR: failed to close shared memory object\n");

    /* Open semaphore */
    if((mutex = sem_open(SEM_NAME, 0)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to open semaphore\n");
        return -1;
    }
    pid = getpid();
    /* Obtain semaphore and increment counter */
    for(i = 0; i < nloop; i++)
    {
        if(sem_wait(mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait\n");
            return -1;
        }
        /* Print counter along with PID and increment */
        printf("pid %ld: %d\n", (long)pid, ptr->count++);
        /* Let other clients run */
        sleep(1);
        if(sem_post(mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post\n");
            return -1;
        }
    }
    exit(0);
}
/******************************************************************************/