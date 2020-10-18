/******************************************************************************
* Filename              : shm_client_server_server.c
* Author                : Pranit Ekatpure
* Description           : Producer-consumer example using Shared Memory. This 
*                         contains server implementation. A server is started
*                         that creates a shared memory object in which messages
*                         are placed by client processes. The server just prints
*                         these messages.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "shm_cliserv.h"

/******************************************************************************
* Macros
*******************************************************************************/
#define SHARED_FILE "temp"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for server that fetches and prints the messages
*                 from shared memory.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int fd, index, lastnoverflow, temp;
    long offset;
    struct shmstruct *ptr;

    /* Create shm, set its size, map it, close descriptor */
    /* Unlink shared memory object in case it is exists 
     * OK if this fails */
    shm_unlink(SHARED_FILE);
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

    /* Initialize the array of offsets */
    for(index = 0; index < NMESG; index++)
        ptr->msgoff[index] = index * MESGSIZE;
    
    /* Initialize the semaphores in shared memory */
    if(sem_init(&ptr->mutex, 1, 1) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init mutex\n");
        return -1;
    }
    if(sem_init(&ptr->nempty, 1, NMESG) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nempty\n");
        return -1;
    }
    if(sem_init(&ptr->nstored, 1, 0) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init nstored\n");
        return -1;
    }
    if(sem_init(&ptr->noverflowmutex, 1, 1) == -1)
    {
        fprintf(stderr, "ERROR: failed sem_init noverflowmutex\n");
        return -1;
    }

    index = 0;
    lastnoverflow = 0;
    for( ; ; )
    {
        /* Wait for message, and then print */
        if(sem_wait(&ptr->nstored) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait nstored\n");
            return -1;
        }
        if(sem_wait(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait mutex\n");
            return -1;
        }
        offset = ptr->msgoff[index];
        printf("Index = %d: %s\n", index, &ptr->msgdata[offset]);
        /* circular buffer */
        if(++index >= NMESG)
            index = 0;    

        if(sem_post(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post mutex\n");
            return -1;
        }
        if(sem_post(&ptr->nempty) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post nempty\n");
            return -1;
        }

        /* Handle overflows */
        if(sem_wait(&ptr->noverflowmutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait noverflowmutex\n");
            return -1;
        }
        /* don't printf while mutex held */
        temp = ptr->noverflow;      
        if(sem_post(&ptr->noverflowmutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post noverflowmutex\n");
            return -1;
        }
        if(temp != lastnoverflow)
        {
            printf("noverflow = %d \n", temp);
            lastnoverflow = temp;
        }
    }
    exit(0);
}
/******************************************************************************/