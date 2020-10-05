/******************************************************************************
* Filename              : client.c
* Author                : Pranit Ekatpure
* Description           : This is file contains client function.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "mesg.h"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : client
* Description   : Client function that uses messages.
*
* Parameters    :
*   int readfd  : read fd
*   int writefd : write fd    
* Return value  : void
*
*******************************************************************************/
void client(int readfd, int writefd)
{
    size_t len;
    ssize_t n;
    struct mymesg mesg;

    printf("Client: Enter name of file to be read by server - ");

    /* read pathname */
    if(fgets(mesg.mesg_data, MAXMESGDATA, stdin) < 0)
    {
        fprintf(stderr, "ERROR: failed to read pathname\n");
        return;
    }

    len = strlen(mesg.mesg_data);
    if(mesg.mesg_data[len - 1] == '\n')
        len--;                              /* delete newline from fgets() */
    mesg.mesg_len = len;
    mesg.mesg_type = 1;

    /* write pathname to IPC channel */
    if(mesg_send(writefd, &mesg) < 0)
    {
        fprintf(stderr, "ERROR: failed to write pathname to IPC channel\n");
        return;
    }

    /* read from IPC, write to standard output */
    while(1)
    {
        if((n = mesg_recv(readfd, &mesg)) < 0)
        {
            fprintf(stderr, "ERROR: mesg_recv error\n");
            return;
        }
        else if(n == 0)
        {
            /* End of file */
            return;
        }

        if(write(STDOUT_FILENO, mesg.mesg_data, n) != n)
        {
            fprintf(stderr, "ERROR: write error\n");
            return;
        }
    }    
}
/******************************************************************************/