/* A server is started that creates a shared memory object in which messages are placed by 
client processes. The server just prints these messages. */

#include "shm_cliserv.h"

#define SHARED_FILE "temp"

int main()
{
    int fd, index, lastnoverflow, temp;
    long offset;
    struct shmstruct *ptr;

        /* create shm, set its size, map it, close descriptor */
    shm_unlink(SHARED_FILE);
    fd = shm_open(SHARED_FILE, O_RDWR | O_CREAT | O_EXCL, 0666);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ftruncate(fd, sizeof(struct shmstruct));
    close(fd);

        /* initialize the array of offsets */
    for(index = 0; index < NMESG; index++)
        ptr->msgoff[index] = index * MESGSIZE;
    
        /* initialize the semaphores in shared memory */
    sem_init(&ptr->mutex, 1, 1);
    sem_init(&ptr->nempty, 1, NMESG);
    sem_init(&ptr->nstored, 1, 0);
    sem_init(&ptr->noverflowmutex, 1, 1);

        /* this program is the consumer */
    index = 0;
    lastnoverflow = 0;
    for( ; ; )
    {
        sem_wait(&ptr->nstored);
        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[index];
        printf("index = %d: %s\n", index, &ptr->msgdata[offset]);
        if(++index >= NMESG)
            index = 0;          /* circular buffer */
        sem_post(&ptr->mutex);
        sem_post(&ptr->nempty);

        sem_wait(&ptr->noverflowmutex);
        temp = ptr->noverflow;      /* don't printf while mutex held */
        sem_post(&ptr->noverflowmutex);
        if(temp != lastnoverflow);
        {
            printf("noverflow = %d \n", temp);
            lastnoverflow = temp;
        }
    }
    exit(0);
}