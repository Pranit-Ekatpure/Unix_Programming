/******************************************************************************
* Filename              : mqgetattr.c
* Author                : Pranit Ekatpure
* Description           : Message Queue get attribute Example.
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
* Description   : main function for Message Queue get attribute Example. It 
*                 opens a specified Message Queue and prints its attributes.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    mqd_t mqd;
    struct mq_attr attr;

    /* Creat Message Queue */
    if((mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, FILE_MODE, NULL)) == -1)
    {
        fprintf(stderr,"ERROR: failed to create msg queue\n");
        exit(1);
    }

    /* Get Message Queue Attributes */
    if(mq_getattr(mqd, &attr) == -1)
    {
        fprintf(stderr,"ERROR: failed to get msg queue attributes\n");
        exit(1);
    }

    /* Print attributes */
    printf("Max msgs: %ld, Max bytes/msg: %ld, Currently msgs on Queue: %ld",
            attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
            
    /* close Message Queue */
    if(mq_close(mqd) == -1)
    {
        fprintf(stderr,"ERROR: failed to close msg queue\n");
        exit(1);
    }
    exit(0);
}
/******************************************************************************/

