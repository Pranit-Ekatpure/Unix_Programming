/* Multiple producer multiple consumer problem */
/* standard includes */
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/* globals */
#define NBUFF  10
#define MAXNTHREADS 100

int nitems, nproducers, nconsumers;         /* read-only */

struct                                      /* data shared by producer and consumer */
{
    int buff[NBUFF];            
    int nput;                               /* item number: 0, 1, 2,.... */
    int nputval;                            /* value to store in buff[] */
    int nget;                               /* item number: 0, 1, 2,.... */
    int ngetval;                            /* value to store in buff[] */
    sem_t mutex, nempty, nstored;           /* semaphores */
}shared;

void *produce(void *), *consume(void *);

int main()
{
    int i, prodcount[MAXNTHREADS], conscount[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume[MAXNTHREADS];

    nproducers = 10;
    nconsumers = 10;
    nitems = 1000000;

        /* initialize three semaphores */
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);

        /* create all producers and all consumers */
    for(i = 0; i < nproducers; i++)
    {
        prodcount[i] = 0;
        pthread_create(&tid_produce[i], NULL, produce, &prodcount[i]);
    }
    
    for(i = 0; i < nconsumers; i++)
    {
        conscount[i] = 0;
        pthread_create(&tid_consume[i], NULL, consume, &conscount[i]);
    }

        /* wait for producers and consumers */
    for(i = 0; i < nproducers; i++)
    {
        pthread_join(tid_produce[i], NULL);
        /* print how many time each producer thread run */
        printf("producer count[%d] = %d\n", i, prodcount[i]);
    }
    printf("\n");
    for(i = 0; i < nconsumers; i++)
    {
        pthread_join(tid_consume[i], NULL);
        /* print how many time each consumer thread run */
        printf("consumer count[%d] = %d\n", i, conscount[i]);
    }

        /* delete semaphores */
    sem_destroy(&shared.mutex);
    sem_destroy(&shared.nempty);
    sem_destroy(&shared.nstored);
    exit(0);
}

void *produce(void *arg)
{
    for( ; ; )
    {
        sem_wait(&shared.nempty);           /* wait for at least 1 empty slot */
        sem_wait(&shared.mutex);

        if(shared.nput >= nitems)
        {
            sem_post(&shared.nstored);      /* let consumers terminate */
            sem_post(&shared.nempty);
            sem_post(&shared.mutex);
            return (NULL);                  /* all done */
        }
        shared.buff[shared.nput % NBUFF] = shared.nputval;
        shared.nput++;
        shared.nputval++;

        sem_post(&shared.mutex);
        sem_post(&shared.nstored);          /* 1 more stored item */
        *((int *) arg) += 1;
    }
}

void *consume(void *arg)
{
    for( ; ; )
    {
        int i;

        sem_wait(&shared.nstored);          /* wait for at least 1 stored item */
        sem_wait(&shared.mutex);

        if(shared.nget >= nitems)
        {
            sem_post(&shared.nstored);
            sem_post(&shared.mutex);
            return (NULL);                  /* all done */
        }
        i = shared.nget % NBUFF;
        if(shared.buff[i] != shared.ngetval)
            printf("error: buff[%d] = %d\n", i, shared.buff[i]);
        shared.nget++;
        shared.ngetval++;

        sem_post(&shared.mutex);
        sem_post(&shared.nempty);           /* 1 more empty slot */
        *((int *) arg) += 1;
    }
}
