#include "unpipc.h"

#define MQ_NAME "/myque.1"

    mqd_t mqd;
    char *buff;
    struct mq_attr attr;
    struct sigevent sigev;

    static void sig_usr1(int);

int main()
{
        /* open queue, get attributes, allocate read buffer */
    mqd = mq_open(MQ_NAME, O_RDONLY);
    mq_getattr(mqd, &attr);

    buff = malloc(attr.mq_msgsize);

        /* establish signal handler, enable notification */
    signal(SIGUSR1, sig_usr1);
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    if(mq_notify(mqd, &sigev) == -1)
        printf("mq_notify failed...");

    while(1);
    
    // for( ; ;)
        // pause();            /* signal handler does everything */
    
    exit(0);
}

static void sig_usr1(int signo)
{
    ssize_t n;

    mq_notify(mqd, &sigev);         /* reregister first */
    n = mq_receive(mqd, buff, attr.mq_msgsize, NULL);
    printf("SIGUSR1 received, read %ld bytes\r\n", (long)n);
    return;
}