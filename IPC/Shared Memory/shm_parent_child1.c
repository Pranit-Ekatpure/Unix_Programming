/* This example is same as shm_parent_child.c. Here Posix memory based semaphore is used
instead of a Posix named semaphore. */

#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHARED_FILE     "temp"

struct shared
{
    sem_t mutex;        /* the mutex: a Posix mamory-based semaphore */
    int count;          /* and the counter */
}shared;

int main()
{
    int fd, i, nloop;;
    struct shared *ptr;
    
    nloop = 10000;

        /* open file, initialize to 0, map into memory */
    fd = open(SHARED_FILE, O_RDWR | O_CREAT, 0666);
    write(fd, &shared, sizeof(struct shared));
    ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

        /* initialize semaphore that is shared between processes */
    sem_init(&ptr->mutex, 1, 1);
    
    setbuf(stdout, NULL);       /* stdout is unbuffered */
    if(fork() == 0)             /* child */
    {
        for(i = 0; i < nloop; i++)
        {
            sem_wait(&ptr->mutex);
            printf("child: %d\n", ptr->count++);
            sem_post(&ptr->mutex);
        }
        exit(0);
    }
        /* parent */
    for(i = 0; i < nloop; i++)
    {
        sem_wait(&ptr->mutex);
        printf("parent: %d\n", ptr->count++);
        sem_post(&ptr->mutex);
    }
    exit(0);
}

