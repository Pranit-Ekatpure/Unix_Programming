/******************************************************************************
* Filename              : server.c
* Author                : Pranit Ekatpure
* Description           : This contains Shared Counter example's server 
*                         implementation.
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
* Variable Definitions
*******************************************************************************/
/* Struct stored in shared memory */
struct shmstruct                
{
    int count;
};
/* Pointer to named semaphore */
sem_t *mutex;                   

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for server that creates and initializes shared
*                 memory ans semphore.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int fd;
    struct shmstruct *ptr;
    /* Unlink shared memory object in case it is exists 
     * OK if this fails */
    shm_unlink(SHARED_FILE);

    /* Create shared memory object, set its size, map it, close descriptor */
    if((fd = shm_open(SHARED_FILE, O_RDWR | O_CREAT | O_EXCL, 0666)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create shared memory object\n");
        return -1;
    }
    if(ftruncate(fd, sizeof(struct shmstruct)) == -1)
    {
        fprintf(stderr, "ERROR: failed ftruncate\n");
        return -1;
    }
    if((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, 
        MAP_SHARED, fd, 0)) == MAP_FAILED)
        {
            fprintf(stderr, "ERROR: failed to mmap\n");
            return -1;
        }
    if(close(fd) == -1)
        fprintf(stderr, "ERROR: failed to close shared memory object\n");
    
    /* Create and initialize semaphore */
    /* Unlink semaphore in case it is exists 
     * OK if it fails */
    sem_unlink(SEM_NAME);       
    if((mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "ERROR: failed to create semaphore\n");
        return -1;
    }
    if(sem_close(mutex) == -1)
        fprintf(stderr, "ERROR: failed sem_close mutex\n");
    
    exit(0);
}
/******************************************************************************/