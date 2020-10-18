/******************************************************************************
* Filename              : shm_parent_child.c
* Author                : Pranit Ekatpure
* Description           : This file contain example of memory sharing between 
*                         the parent and the child process. The parent and child 
*                         share a piece of memory in which counter is stored. 
*                         To do so, we use a memory mapped file: a file that we 
*                         open and then mmap into our address space.
*******************************************************************************/

/******************************************************************************
* Includes
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
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for memory sharing between parent and child.
*                 A counter is stored in shared memory which is increased by
*                 both parent and child.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int fd, i, nloop, zero = 0;
    int *ptr;
    sem_t *mutex;
    nloop = 10000;

    /* open file, initialize to 0, map into memory */
    if((fd = open(SHARED_FILE, O_RDWR | O_CREAT, 0666)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create file\n");
        return -1;
    }
    if(write(fd, &zero, sizeof(int)) != sizeof(int))
    {
        fprintf(stderr, "ERROR: failed write to file\n");
        return -1;
    }
    if((ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) 
        == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: failed to map file into address space\n");
        return -1;
    }
    if(close(fd) == -1)
    {
        fprintf(stderr, "ERROR: failed to close file\n");
        return -1;
    }
    /* Create and initialize semaphore */
    if((mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to create semaphore\n");
        return -1;
    }
    if(sem_unlink(SEM_NAME) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_unlink %s\n", SEM_NAME);
    }
    /* unbuffer stdout */
    setbuf(stdout, NULL);      
    /* fork, parent and child both increment the interger counter */     
    if(fork() == 0)                 /* child */
    {
        for(i = 0; i < nloop; i++)
        {
            if(sem_wait(mutex) == -1)
            {
                fprintf(stderr, "ERROR: failed sem_wait\n");
                return -1;
            }
            /* Child: print and increment counter */
            printf("child: %d\n", (*ptr)++);
            if(sem_post(mutex) == -1)
            {
                fprintf(stderr, "ERROR: failed sem_post\n");
                return -1;
            }
        }
        exit(0);
    }
    /* parent */
    for(i = 0; i < nloop; i++)
    {
        if(sem_wait(mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait\n");
            return -1;
        }
        /* Parent: print and increment counter */
        printf("parent: %d\n", (*ptr)++);
        if(sem_post(mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post\n");
            return -1;
        }
    }
    exit(0);
}
/******************************************************************************/