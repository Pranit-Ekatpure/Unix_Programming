/******************************************************************************
* Filename              : mqnotify.c
* Author                : Pranit Ekatpure
* Description           : Message Queue simple signal notification Example.
*                         Program causes new thread to be created when a message
*                         is placed onto a empty Queue.
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
* Variable Definitions
*******************************************************************************/
mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* our thread function */
static void notify_thread(union sigval); 

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : mian
* Description   : main function that demonstrate creation new thread when 
*                 message is placed onto an empty queue.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/

int main()
{
    /* open queue, get attributes, allocate read buffer */
    if((mqd = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK)) == -1)
    {
        fprintf(stderr, "ERROR: mq_open failed\n");
        return -1;
    }
    if(mq_getattr(mqd, &attr) == -1)
    {
        fprintf(stderr, "ERROR: mq_getattr failed\n");
        return -1;
    }
    /* Establish notification for SIGEV_THREAD */
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = NULL;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;
    if (mq_notify(mqd, &sigev) == -1)
    {
        fprintf(stderr,"ERROR: mq_notify failed\n");
        return -1;
    }

    /* main in an infinite loop that goes to sleep in the pause function, which return
       each time a event is occured */
    while(1)
        pause();

    exit(0);
}

/******************************************************************************
* Function      : notify_thread
* Description   : Thread function.
* Parameters    : 
*   union sigval arg   : sigvals
* Return value  : void
*
*******************************************************************************/
static void notify_thread(union sigval arg)
{
    ssize_t n;
    char *buff;

    printf("Notify_thread Started\n");
    /* Allocate memory for read buffer */
    if((buff = malloc(attr.mq_msgsize)) == NULL)
    {
        fprintf(stderr, "ERROR: failed to allocate memory\n");
        return;
    }
    /* reregister first for next event */
    mq_notify(mqd, &sigev); /* reregister first */

    /* Read the message and print its lenght */
    while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0)
        printf("Thread, read %ld bytes\r\n", (long)n);

    if (errno != EAGAIN)
        fprintf(stderr, "ERROR:mq_receive error\n");

    /* deallocate memory of read buffer */
    free(buff);
    /* Exit from thread */
    pthread_exit(NULL);
    return;
}
/******************************************************************************/