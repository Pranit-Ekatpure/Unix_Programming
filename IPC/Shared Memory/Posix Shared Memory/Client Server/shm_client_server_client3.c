/* Clients that feeds message in the shares memory */

#include "shm_cliserv.h"

#define SHARED_FILE "temp"

int main()
{
    int fd, i, nloop, nsec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct *ptr;

    nloop = 10000;
    nsec = 1;

        /* open and map shared memory that server must create */
    fd = shm_open(SHARED_FILE, O_RDWR, 0666);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    pid = getpid();
    for(i = 0; i < nloop; i++)
    {
        sleep(nsec);
        snprintf(mesg, MESGSIZE, "pid %ld: message %d", (long)pid, i);
        if(sem_trywait(&ptr->nempty) == -1)
        {
            if(errno == EAGAIN)
            {
                sem_wait(&ptr->noverflowmutex);
                ptr->noverflow++;
                sem_post(&ptr->noverflowmutex);
                continue;
            }
            else
            {
                printf("sem_trywait error");
            }
            
        }
        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[ptr->nput];
        if(++(ptr->nput) >= NMESG)
            ptr->nput = 0;              /* circular buffer */
        sem_post(&ptr->mutex);
        strcpy(&ptr->msgdata[offset], mesg);
        sem_post(&ptr->nstored);
    }
    exit(0);
}