/******************************************************************************
* Filename              : shm_client_server_client3.c
* Author                : Pranit Ekatpure
* Description           : Producer-consumer example using Shared Memory. This 
*                         contains client implementation. The clinet feeds
*                         message in the shared memory.
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
* Description   : main function for client that stores messages in shard 
*                 memory for server.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int fd, i, nloop, nsec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct *ptr;

    nloop = 10000;
    nsec = 1;

    /* Open and map shared memory that server must create */
    if((fd = shm_open(SHARED_FILE, O_RDWR, 0666)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open shared memory object\n");
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

    pid = getpid();
    /* Store messages */
    for(i = 0; i < nloop; i++)
    {
        sleep(nsec);
        snprintf(mesg, MESGSIZE, "pid %ld: message %d", (long)pid, i);
        /* sem_trywait, if value of semaphore is 0, an error of EAGAIN is 
         * returned. We detect this error and increment the overflow counter */
        if(sem_trywait(&ptr->nempty) == -1)
        {
            if(errno == EAGAIN)
            {
                if(sem_wait(&ptr->noverflowmutex) == -1)
                {
                    fprintf(stderr, "ERROR: failed sem_wait noverflowmutex\n");
                    return -1;
                }
                ptr->noverflow++;
                if(sem_post(&ptr->noverflowmutex) == -1)
                {
                    fprintf(stderr, "ERROR: failed sem_post noverflowmutex\n");
                    return -1;
                }
                continue;
            }
            else
            {
                printf("sem_trywait error");
            }
            
        }
        
        if(sem_wait(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_wait mutex\n");
            return -1;
        }
        offset = ptr->msgoff[ptr->nput];
        /* circular buffer */
        if(++(ptr->nput) >= NMESG)
            ptr->nput = 0;              
        if(sem_post(&ptr->mutex) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post mutex\n");
            return -1;
        }
        /* Copy message into shared memory */
        strcpy(&ptr->msgdata[offset], mesg);
        /* One more message stored */
        if(sem_post(&ptr->nstored) == -1)
        {
            fprintf(stderr, "ERROR: failed sem_post nstored\n");
            return -1;
        }
    }
    exit(0);
}
/******************************************************************************/