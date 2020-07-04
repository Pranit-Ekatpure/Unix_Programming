#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define NBUFF 10

int nitems;         /* read-only by producer and consumer */
struct {
    int buff[NBUFF];
    sem_t mutex, nempty, nstored;
}shared;

void *produce(void *), *consume(void *);

int main()
{
    pthread_t tid_produce, tid_consume;

    nitems = 1000000;

        /* initialize three semaphores */
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);

        /* create threads: producer and consumer */
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

        /* wait for threads termination */
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

        /* remove semaphores */
    sem_destroy(&shared.mutex);
    sem_destroy(&shared.nempty);
    sem_destroy(&shared.nstored);

    exit(0);
}

void *produce(void *arg)
{
    int i;

    for(i = 0; i < nitems; i++)
    {
        sem_wait(&shared.nempty);           /* wait for at least 1 empty slot */
        sem_wait(&shared.mutex);
        shared.buff[i % NBUFF] = i;         /* store i into circular buffer */
        sem_post(&shared.mutex);
        sem_post(&shared.nstored);          /* 1 more item stored */
    }
    return(NULL);
}

void *consume(void *arg)
{
    int i;

    for(i = 0; i < nitems; i++)
    {
        sem_wait(&shared.nstored);          /* wait for at least 1 stored item */
        sem_wait(&shared.mutex);
        if(shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d \n", i % NBUFF, shared.buff[i % NBUFF]);
        sem_post(&shared.mutex);
        sem_post(&shared.nempty);           /* 1 more empty slot */
    }
    return(NULL);
}