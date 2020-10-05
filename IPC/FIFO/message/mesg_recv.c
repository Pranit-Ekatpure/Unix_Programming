/******************************************************************************
* Filename              : mesg_recv.c
* Author                : Pranit Ekatpure
* Description           : This is file contains mesg_recv function definition.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "mesg.h"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : mesg_recv
* Description   : Message receive function.
*
* Parameters    :
*   int fd              : File descriptor
*   struct mymesg *mptr : Pointer to our own message structure
* Return value  : ssize_t
*
*******************************************************************************/
ssize_t mesg_recv(int fd, struct mymesg *mptr)
{
    size_t len;
    ssize_t n;

    /* read message header first, to get len of data that follows */
    if((n = read(fd, mptr, MESGHDRSIZE)) == 0)
        return 0;             /* end of file */
    else if(n != MESGHDRSIZE)
        fprintf(stderr,"ERROR: message header: expected %ld, got %ld", MESGHDRSIZE, n);

    if((len = mptr->mesg_len) > 0)
        if((n = read(fd, mptr->mesg_data, len)) != len)
            fprintf(stderr,"ERROR: message data: expected %ld, got %ld", len, n);

    return (len);
}
/******************************************************************************/