#include "unpipc.h"

#define MQ_NAME "/myque.1"

int main()
{
    mqd_t mqd;
    char *buff;
    size_t n;
    unsigned int prio;
    struct mq_attr attr;

    mqd = mq_open(MQ_NAME, O_RDONLY);
    mq_getattr(mqd, &attr);

    buff = malloc(attr.mq_msgsize);

    if((n = mq_receive(mqd, buff, attr.mq_msgsize, &prio)) == -1)
    {
        printf("mq_receive failed...");
        exit(1);
    }
  
    printf("read %ld bytes, priority = %u\r\n", (long)n, prio);

    exit(0);
}