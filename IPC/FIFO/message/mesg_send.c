/******************************************************************************
* Filename              : mesg_send.c
* Author                : Pranit Ekatpure
* Description           : This is file contains mesg_send function definition.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "mesg.h"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : mesg_send
* Description   : Message send function
*
* Parameters    :
*   int fd              : File descriptor
*   struct mymesg *mptr : Pointer to our own message structure
* Return value  : ssize_t
*
*******************************************************************************/
ssize_t mesg_send(int fd, struct mymesg *mptr)
{
    return (write(fd, mptr, MESGHDRSIZE + mptr->mesg_len));
}
/******************************************************************************/