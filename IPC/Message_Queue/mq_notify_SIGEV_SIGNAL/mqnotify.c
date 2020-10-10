/******************************************************************************
* Filename              : mqnotify.c
* Author                : Pranit Ekatpure
* Description           : Message Queue simple signal notification Example.
*                         Program causes SIGUSR1 to be generated when a message
*                         is place on to empty Queue.
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
/* Global variables that are used by both main function and signal handler */
mqd_t mqd;
char *buff;
struct mq_attr attr;
struct sigevent sigev;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void sig_usr1(int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : mian
* Description   : main function that demonstrate generation of signal SIGUSR1 
*                 when message is placed onto an empty queue.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    /* open queue, get attributes, allocate read buffer */
    if((mqd = mq_open(MQ_NAME, O_RDONLY)) == -1)
    {
        fprintf(stderr, "ERROR: mq_open failed\n");
        return -1;
    }
    if(mq_getattr(mqd, &attr) == -1)
    {
        fprintf(stderr, "ERROR: mq_getattr failed\n");
        return -1;
    }
    if((buff = malloc(attr.mq_msgsize)) == NULL)
    {
        fprintf(stderr, "ERROR: failed to allocate memory for buffer\n");
        return -1;
    }

    /* Establish signal handler for SIGUSR1,
     * Enable notification for event when empty Message Queue gets message message */
    signal(SIGUSR1, sig_usr1);
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    if(mq_notify(mqd, &sigev) == -1)
    {
        fprintf(stderr,"ERROR: mq_notify failed\n");
        return -1;
    }

    /* main in an infinite loop that goes to sleep in the pause function, which return
       each time a signal is caught */
    while(1)
        pause();
   
    exit(0);
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : sig_usr1
* Description   : Signal handler function.
* Parameters    : 
*   int signo   : signal number
* Return value  : void
*
*******************************************************************************/
static void sig_usr1(int signo)
{
    ssize_t n;
    /* reregister first for next event*/
    if(mq_notify(mqd, &sigev) == -1)
    {
        fprintf(stderr,"ERROR: mq_notify failed\n");
        return;
    }  
    /* Read the message and print its lenght */       
    if((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) == -1)
    {
        fprintf(stderr, "ERROR: mq_receive failed\n");
        return;
    }
    printf("SIGUSR1 received, read %ld bytes\r\n", (long)n);
    return;
}
/******************************************************************************/