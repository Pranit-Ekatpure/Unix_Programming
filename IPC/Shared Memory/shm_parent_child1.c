/******************************************************************************
* Filename              : shm_parent_child.c
* Author                : Pranit Ekatpure
* Description           : This file contain example of memory sharing between 
*                         the parent and the child process. The parent and child 
*                         share a piece of memory in which counter is stored. 
*                         To do so, we use a memory mapped file: a file that we 
*                         open and then mmap into our address space.
*
* This example is same as shm_parent_child.c. Here Posix memory based semaphore 
* is used instead of a Posix named semaphore, and store this semaphore in the
* shared memory. 
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
#define SHARED_FILE     "temp"

/******************************************************************************
* Variable Definitions
*******************************************************************************/
struct shared
{
    /* The mutex: a Posix mamory-based semaphore */
    sem_t mutex;    
    /* The counter */    
    int count;          
}shared;

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
int main()
{
    int fd, i, nloop;;
    struct shared *ptr;
    
    nloop = 10000;

    /* open file, initialize to 0, map into memory */
    if((fd = open(SHARED_FILE, O_RDWR | O_CREAT, 0666)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create file\n");
        return -1;
    }
    if(write(fd, &shared, sizeof(struct shared)) != sizeof(struct shared))
    {
        fprintf(stderr, "ERROR: failed write to file\n");
        return -1;
    }
    if((ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, 
        fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: failed to map file into address space\n");
        return -1;
    }
    if(close(fd) == -1)
    {
        fprintf(stderr, "ERROR: failed to close file\n");
        return -1;
    }

    /* Initialize semaphore that is shared between processes */
    if(sem_init(&ptr->mutex, 1, 1) == -1)
    {
        fprintf(stderr, "ERROR: failed to create semaphore\n");
        return -1;
    }
    /* unbuffer stdout */
    setbuf(stdout, NULL); 

    /* fork, parent and child both increment the interger counter */      
    if(fork() == 0)             /* child */
    {
        for(i = 0; i < nloop; i++)
        {
            if(sem_wait(&ptr->mutex) == -1)
            {
                fprintf(stderr, "ERROR: failed sem_wait\n");
                return -1;
            }
            /* Child: print and increment counter */
            printf("child: %d\n", ptr->count++);
            if(sem_post(&ptr->mutex) == -1)
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
        if(sem_wait(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait\n");
            return -1;
        }
        /* Parent: print and increment counter */
        printf("parent: %d\n", ptr->count++);
        if(sem_post(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post\n");
            return -1;
        }
    }
    exit(0);
}
/******************************************************************************/

