#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXNITEMS        1000000
#define MAXNTHREADS      100

    /* globals shared by threads */
int nitems;             /* read-only by producer and consumer */
int buff[MAXNITEMS];

struct{
    pthread_mutex_t mutex;
    int nput;                   /* next index to store */
    int nval;                   /* next value to store */
} put = {
    PTHREAD_MUTEX_INITIALIZER
};

struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready;                 /* number ready for consumer */
}nready = {
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER
};

void *produce(void*), *consume(void*);

int main(){
    int i, nthreads, count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    nitems = MAXNITEMS;         /* the number of items for the producers to store */
    nthreads = 5;               /* the number of producer threads to create */

        /* start all the producer threads */
    for(i = 0; i < nthreads; i++){
        count[i] = 0;
        pthread_create(&tid_produce[i], NULL, produce, &count[i]); // the argument to each producer thread is a pointer
                                                                   // to an element of the count array. we first initialize
                                                                   // the counter to 0, and each thread then increment this
                                                                   // counter each time it stores an item in the buffer.
                                                                   // we print this array of counters when we are done, to 
                                                                   // see how many items were stored by each producer.
    }

        /* start the consumer thread */
    pthread_create(&tid_consume, NULL, consume, NULL);

        /* wait for all the producer threads */
    for(i = 0; i < nthreads; i++){
        pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

        /* wait for the consumer thread */
    pthread_join(tid_consume, NULL);
    exit(0);
}

void *produce(void *arg)
{
    for( ; ; ){
        pthread_mutex_lock(&put.mutex);
        if(put.nput >= nitems){
            pthread_mutex_unlock(&put.mutex);
            return (NULL);          /* array is full, we're done */
        }
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        pthread_mutex_unlock(&put.mutex);

        pthread_mutex_lock(&nready.mutex);
        if(nready.nready == 0)
            pthread_cond_signal(&nready.cond);
        nready.nready++;
        pthread_mutex_unlock(&nready.mutex);

        *((int*)arg) += 1;
    }
}

void *consume(void *arg)
{
    int i;
    for(i = 0; i < nitems; i++){
        pthread_mutex_lock(&nready.mutex);
        while(nready.nready == 0)
            pthread_cond_wait(&nready.cond, &nready.mutex);
        nready.nready--;
        pthread_mutex_unlock(&nready.mutex);

        if(buff[i] != i)
            printf("buff[%d] = %d\n", i, buff[i]);
    }
    return (NULL);
}