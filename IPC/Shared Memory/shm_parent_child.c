/* The parent and child share a piece of memory in which counter is stored.
To do so, we use a memory mapped file: a file that we open and then mmap into
our address space.*/

#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SEM_NAME        "/mysem"
#define SHARED_FILE     "temp"

int main()
{
    int fd, i, nloop, zero = 0;
    int *ptr;
    sem_t *mutex;
    nloop = 10000;

        /* open file, initialize to 0, map into memory */
    fd = open(SHARED_FILE, O_RDWR | O_CREAT, 0666);
    if(fd == -1)
    {
        printf("Unable to open shared file...\n");
        exit(0);
    }
    write(fd, &zero, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    sem_unlink(SEM_NAME);
    setbuf(stdout, NULL);           /* stdout is unbuffered */
    if(fork() == 0)                 /* child */
    {
        for(i = 0; i < nloop; i++)
        {
            sem_wait(mutex);
            printf("child: %d\n", (*ptr)++);
            sem_post(mutex);
        }
        exit(0);
    }
        /* parent */
    for(i = 0; i < nloop; i++)
    {
        sem_wait(mutex);
        printf("parent: %d\n", (*ptr)++);
        sem_post(mutex);
    }
    exit(0);
}