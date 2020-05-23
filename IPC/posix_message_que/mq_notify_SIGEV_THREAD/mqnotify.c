#include "unpipc.h"

#define MQ_NAME "/myque.1"

    mqd_t mqd;
    struct mq_attr attr;
    struct sigevent sigev;

    static void notify_thread(union sigval);        /* our thread function */

int main()
{
        /* open queue, get attributes, allocate read buffer */
    mqd = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
    mq_getattr(mqd, &attr);

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = NULL;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;
    if(mq_notify(mqd, &sigev) == -1)
        printf("mq_notify failed...");

    while(1);
   
    exit(0);
}

static void notify_thread(union sigval arg)
{
    ssize_t n;
    char *buff;

    printf("notify_thread started\n");
    buff = malloc(attr.mq_msgsize);
    mq_notify(mqd, &sigev);         /* reregister first */
    
    while((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0)
        printf("thread, read %ld bytes\r\n", (long)n);
    
    if(errno != EAGAIN)
        printf("mq_receive error");
    
    free(buff);
    pthread_exit(NULL);
    return;
}