#include "unpipc.h"

#define MQ_NAME "/myque.1"

int main()
{
    int c, flags;
    mqd_t mqd;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 128;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;    

    flags = O_RDWR | O_CREAT;

    if(mq_open(MQ_NAME, flags, FILE_MODE, &attr) == -1)
        printf("failed to create msg queue");
       
    mq_close(mqd);
    exit(0);
}
