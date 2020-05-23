#include "unpipc.h"

#define MQ_NAME "/myque.1"

int main()
{
    mqd_t mqd;
    struct mq_attr attr;

    mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, FILE_MODE, NULL);

    mq_getattr(mqd, &attr);
    printf("max #msgs = %ld, max #bytes/msg = %ld #currently on queue = %ld",
            attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    mq_close(mqd);      

    exit(0);
}
