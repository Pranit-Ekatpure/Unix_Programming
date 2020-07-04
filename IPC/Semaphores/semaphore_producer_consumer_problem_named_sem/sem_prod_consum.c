#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define NBUFF       10
#define SEM_MUTEX   "/mutex"
#define SEM_NEMPTY  "/nempty"
#define SEM_NSTORED "/nstored"

int nitems;         /* read-only by producer and consumer */
struct{
    int buff[NBUFF];
    sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *), *consume(void *);

int main()
{
    pthread_t tid_produce, tid_consume;
    nitems = 100;

        /* create three semaphore */
    shared.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0666, 1);
    shared.nempty = sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, 0666, NBUFF);
    shared.nstored = sem_open (SEM_NSTORED, O_CREAT | O_EXCL, 0666, 0);

        /* create one producer thread and one consumer thread */
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

        /* wait for the two threads */
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

        /* remove the semaphore */
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_NEMPTY);
    sem_unlink(SEM_NSTORED);
    exit(0);
}

void *produce(void *arg)
{
    int i;
    for(i = 0; i < nitems; i++)
    {
        sem_wait(shared.nempty);            /* wait for at least 1 empty slot */
        sem_wait(shared.mutex);
        shared.buff[i % NBUFF] = i;         /* store i into circular buffer */
        sem_post(shared.mutex);
        sem_post(shared.nstored);           /* 1 more stored item */
    }
    return (NULL);
}
 
void *consume(void *arg)
{
    int i;
    for(i = 0; i < nitems; i++)
    {
        sem_wait(shared.nstored);           /* wait for at least 1 stored items */
        sem_wait(shared.mutex);
        if(shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n",i % NBUFF, shared.buff[i % NBUFF]);
        sem_post(shared.mutex);
        sem_post(shared.nempty);            /* 1 more empty slot */
    }
    return (NULL);
}