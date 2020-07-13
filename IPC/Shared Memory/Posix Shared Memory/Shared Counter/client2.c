/* Program that increments a counter in shared memory */

#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct shmstruct                /* struct stored in shared memory */
{
    int count;
};
sem_t *mutex;                   /* pointer to named semaphore */

#define SEM_NAME        "/mysem"
#define SHARED_FILE     "temp"

int main()
{
    int fd, i, nloop;
    pid_t pid;
    struct shmstruct *ptr;    
    nloop = 10000;

    fd = shm_open(SHARED_FILE, O_RDWR, 0666);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_WRITE | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    mutex = sem_open(SEM_NAME, 0);
    pid = getpid();
    for(i = 0; i < nloop; i++)
    {
        sem_wait(mutex);
        printf("pid %ld: %d\n", (long)pid, ptr->count++);
        sem_post(mutex);
    }
    exit(0);
}