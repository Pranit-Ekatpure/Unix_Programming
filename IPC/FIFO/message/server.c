/******************************************************************************
* Filename              : server.c
* Author                : Pranit Ekatpure
* Description           : This is file contains server function.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "mesg.h"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : server
* Description   : Server function that uses messages.
*
* Parameters    :
*   int readfd  : read fd
*   int writefd : write fd    
* Return value  : void
*
*******************************************************************************/
void server(int readfd, int writefd)
{
    FILE *fp;
    ssize_t n;
    char *rptr;
    struct mymesg mesg;

    /* read pathname from IPC channel */
    mesg.mesg_type = 1;
    if((n = mesg_recv(readfd, &mesg)) == 0)
        fprintf(stderr,"ERROR: pathname missing\n");

    /* null terminate pathname */
    mesg.mesg_data[n] = '\0';           

    /* open the file whose pathname is received from client */
    if(( fp = fopen(mesg.mesg_data, "r")) == NULL)
    {
        /* error: must tell client */
        snprintf(mesg.mesg_data + n, sizeof(mesg.mesg_data) - n, ":can't open, %s\n",
                 strerror(errno));
        mesg.mesg_len = strlen(mesg.mesg_data);
        if(mesg_send(writefd, &mesg) != (MESGHDRSIZE + mesg.mesg_len))
        {
            fprintf(stderr, "ERROR: mesg_send error\n");
            return;
        }
    }
    else
    {
        /* fopen succeeded; copy file to IPC channel */
        while (rptr != NULL)
        {
            /* read the file */
            if((rptr = fgets(mesg.mesg_data, MAXMESGDATA, fp)) == NULL && ferror(fp))
            {
                fprintf(stderr, "ERROR: error in reading file\n");
                return;
            }
            mesg.mesg_len = strlen(mesg.mesg_data);
            if(mesg_send(writefd, &mesg) != (MESGHDRSIZE + mesg.mesg_len))
            {
                fprintf(stderr, "ERROR: mesg_send error\n");
                return;
            }
        }
        /* close file */
        if(fclose(fp) == EOF)
            fprintf(stderr, "ERROR: error in file close\n");        
    }

    /* send a 0-lenth message to signify the end */
    mesg.mesg_len = 0;
    if(mesg_send(writefd, &mesg) != (MESGHDRSIZE + mesg.mesg_len))
    {
        fprintf(stderr, "ERROR: mesg_send error\n");
        return;
    }   
}
/******************************************************************************/