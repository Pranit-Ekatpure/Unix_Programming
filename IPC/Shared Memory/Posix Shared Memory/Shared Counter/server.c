/* Program that creates and initializes shared memory and semaphore */

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
    int fd;
    struct shmstruct *ptr;

    shm_unlink(SHARED_FILE);

        /* create shm, set its size, map it, close descriptor */
    fd = shm_open(SHARED_FILE, O_RDWR | O_CREAT | O_EXCL, 0666);
    if(fd == -1)
    {
        printf("Unable to open file..\n");
        exit(0);
    }
    ftruncate(fd, sizeof(struct shmstruct));
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    sem_unlink(SEM_NAME);       
    mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    sem_close(mutex);
    
    exit(0);
}