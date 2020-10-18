/****************************************************************************
* Filename              : shm_cliserv.h
* Author                : Pranit Ekatpure
* Description           : This is header file client-server example using
*                         shared memory.
*****************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/* Max bytes per message, uncluding null at end */
#define MESGSIZE    256     
/* Max messages */            
#define NMESG       16                  

/* Structure stored in shared memory */
struct shmstruct                        
{
    /* Three Posix memory-based semaphores */
    sem_t mutex;                        
    sem_t nempty;               
    sem_t nstored;  
    /* Index into msgoff[] for next put */
    int nput;      
    /* Overflow by senders */                     
    long noverflow;   
    /* Mutex for noverflow counter */                  
    sem_t noverflowmutex;            
    /* Offset in shared memory of each message */   
    long msgoff[NMESG];      
    /* The actual messages */           
    char msgdata[NMESG * MESGSIZE];     
};
/******************************************************************************/