/******************************************************************************
* Filename              : mqsend.c
* Author                : Pranit Ekatpure
* Description           : Message Queue send Example.
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
* Description   : main function for Message Queue send Example. It adds message
*                 to Queue.
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    mqd_t mqd;
    void *ptr;
    size_t len = 100;
    unsigned int prio = 2;

    /* Open the Message Queue */
    if((mqd = mq_open(MQ_NAME, O_WRONLY)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open Message Queue\n");
        return -1;
    }
    /* Allodcate memory for buffer and initialize with 0 */
    if((ptr = calloc(len, sizeof(char))) == NULL)
    {
        fprintf(stderr, "ERROR: failed to allocate memory\n");
        return -1;
    }
    /* Send Message to Queue*/
    if(mq_send(mqd, ptr, len, prio) == -1)
    {
        fprintf(stderr, "ERROR: mq_send failed\n");
        return -1;
    }  
    exit(0);
}
/******************************************************************************/
