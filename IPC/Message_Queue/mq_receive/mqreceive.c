/******************************************************************************
* Filename              : mqreceive.c
* Author                : Pranit Ekatpure
* Description           : Message Queue receive Example.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "unpipc.h"

/******************************************************************************
* Macros
*******************************************************************************/
#define MQ_NAME "/myque.1"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for Message Queue receive Example. It reads next
*                 message from Queue.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    mqd_t mqd;
    char *buff;
    size_t n;
    unsigned int prio;
    struct mq_attr attr;

    /* Open the Message Queue */
    if((mqd = mq_open(MQ_NAME, O_RDONLY)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open Message Queue\n");
        return -1;
    }
    /* Get attributes of Message Queue */
    if(mq_getattr(mqd, &attr) == -1)
    {
        fprintf(stderr, "ERROR: failed to get attributes of Message Queue\n");
        return -1;
    }
    /* Allodcate memory for message */
    if((buff = malloc(attr.mq_msgsize)) == NULL)
    {
        fprintf(stderr, "ERROR: failed to allocate memory\n");
        return -1;
    }
    /* Read the Message from Queue and print its size and priority */
    if((n = mq_receive(mqd, buff, attr.mq_msgsize, &prio)) == -1)
    {
        fprintf(stderr, "ERROR: mq_receive failed\n");
        return -1;
    }  
    printf("Read %ld bytes, Priority = %u\r\n", (long)n, prio);

    exit(0);
}
/******************************************************************************/