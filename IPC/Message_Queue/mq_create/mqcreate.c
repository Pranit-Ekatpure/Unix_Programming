/******************************************************************************
* Filename              : mqcreate.c
* Author                : Pranit Ekatpure
* Description           : Message Queue Creation Example
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
* Description   : main function for Message Queue Creation Example. It creates
*                 a Message Queue.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    mqd_t mqd;
    struct mq_attr attr; 

    /* Creat Message Queue */
    if((mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, FILE_MODE, NULL)) == -1)
    {
        fprintf(stderr,"ERROR: failed to create msg queue\n");
        exit(1);
    }
    /* close Message Queue */
    if(mq_close(mqd) == -1)
    {
        fprintf(stderr,"ERROR: failed to close msg queue\n");
        exit(1);
    }
    exit(0);
}
/******************************************************************************/
