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
    if((n = read(fd, mptr, MESGHDRSIZE)) < 0)
    {
        fprintf(stderr, "ERROR: read failed\n");
        return -1;
    }
    else if(n == 0)
    {
        /* end of file */
        return 0;
    }                
    else if(n != MESGHDRSIZE)
    {
        fprintf(stderr,"ERROR: message header: expected %ld, got %ld", MESGHDRSIZE, n);
        return -1;
    }
    /* read the actual message */
    if((len = mptr->mesg_len) > 0)
    {
        n = read(fd, mptr->mesg_data, len);
        if(n < 0)
        {
            fprintf(stderr, "ERROR: read failed\n");
            return -1;
        }
        if(n != len)
        {
            fprintf(stderr,"ERROR: message data: expected %ld, got %ld", len, n);
            return -1;
        }
    }

    return (len);
}
/******************************************************************************/