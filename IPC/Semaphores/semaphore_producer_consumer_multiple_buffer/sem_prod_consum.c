/* producer consumer problem using multiple buffer */
/* standard includes */
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* globals */

#define NBUFF       8
#define BUFFSIZE    512

struct{                             /* data shared by producer and consumer */
    struct{
        char data[BUFFSIZE];        /* a buffer */
        ssize_t n;                  /* count of #bytes in the buffer */
    }buff[NBUFF];
    sem_t mutex, nempty, nstored;   /* semaphores */
}shared;

int fd;                             /* input file to copy to stdout */
void *produce(void *), *consume(void *);

int main()
{
    pthread_t tid_produce, tid_consume;

    fd = open("sem_prod_consum.c", O_RDONLY);

        /* initialize three semaphores */
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);

        /* one producer thread, one consumer thread */
    pthread_create(&tid_produce, NULL, produce, NULL);          /* reader thread */
    pthread_create(&tid_consume, NULL, consume, NULL);          /* writer thread */

        /* wait for threads */
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

    sem_destroy(&shared.mutex);
    sem_destroy(&shared.nempty);
    sem_destroy(&shared.nstored);
    exit(0);
}

void *produce(void *arg)
{
    int i;
    for(i = 0; ; )
    {
        sem_wait(&shared.nempty);           /* wait for at least 1 empty slot */
        sem_wait(&shared.mutex);
            /* critical region */
        sem_post(&shared.mutex);

        shared.buff[i].n = read(fd, shared.buff[i].data, BUFFSIZE);
        if(shared.buff[i].n == 0)
        {
            sem_post(&shared.nstored);      /* 1 more stored item */
            return (NULL);
        }
        if(++i >= NBUFF)
            i = 0;                          /* circular buffer */
        
        sem_post(&shared.nstored);          /* 1 more stored ite */
    }
}

void *consume(void *arg)
{
    int i;
    for(i = 0; ; )
    {
        sem_wait(&shared.nstored);          /* wait for at least 1 stored item */
        sem_wait(&shared.mutex);
            /* critical region */
        sem_post(&shared.mutex);

        if(shared.buff[i].n == 0)
            return (NULL);
        write(1, shared.buff[i].data, shared.buff[i].n);
        if(++i >= NBUFF)
            i = 0;                          /* circular buffer */
        
        sem_post(&shared.nempty);           /* 1 more empty slot */
    }
}