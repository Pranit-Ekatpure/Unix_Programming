#include "unpipc.h"

#define MQ_NAME "/myque.1"

int main()
{
    mqd_t mqd;
    void *ptr;
    size_t len = 100;
    unsigned int prio = 2;

    mqd = mq_open(MQ_NAME, O_WRONLY);

    ptr = calloc(len, sizeof(char));
    if(mq_send(mqd, ptr, len, prio) == -1)
        printf("mq_send failed...");

    exit(0);
}