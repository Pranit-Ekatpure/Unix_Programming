#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXNITEMS        1000000
#define MAXNTHREADS     100

int nitems;             /* read-only by producer and consumer */

struct{
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput;
    int nval;
} shared = {
    PTHREAD_MUTEX_INITIALIZER
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
        pthread_mutex_lock(&shared.mutex);
        if(shared.nput >= nitems){
            pthread_mutex_unlock(&shared.mutex);
            return (NULL);          /* array is full, we're done */
        }
        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        pthread_mutex_unlock(&shared.mutex);
        *((int*)arg) += 1;
    }
}

void consume_wait(int i)
{
    for( ; ; ){
        pthread_mutex_lock(&shared.mutex);
        if(i < shared.nput){
            pthread_mutex_unlock(&shared.mutex);
            return;             /* an item is ready */
        }
        pthread_mutex_unlock(&shared.mutex);
    }
}

void *consume(void *arg)
{
    int i;
    for(i = 0; i < nitems; i++){
        consume_wait(i);
        if(shared.buff[i] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i]);
    }
    return (NULL);
}