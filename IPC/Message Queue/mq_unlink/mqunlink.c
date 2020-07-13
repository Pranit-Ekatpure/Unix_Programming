#include "unpipc.h"

#define MQ_NAME "/myque.1"

int main()
{
    mq_unlink(MQ_NAME);

    exit(0);
}
