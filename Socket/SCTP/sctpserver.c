/******************************************************************************
* Filename              : sctpserver.c
* Author                : Pranit Ekatpure
* Description           : This file contain SCTP client-server example's server
*                         implementation.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define BUFFSIZE        512
#define	SERV_PORT       9877
#define	LISTENQ         1024
/* Default number of streams */
#define NUM_STREAMS     10          

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for SCTP streaming echo server.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int sock_fd, msg_flags;
    char readbuf[BUFFSIZE];
    struct sockaddr_in serveraddr, cliaddr;
    struct sctp_sndrcvinfo sri;
    struct sctp_event_subscribe events;
    socklen_t len;
    size_t rd_sz;

    /* Create an SCTP one-to-many-style socket */
    if((sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create a socket\n");
        return -1;
    }

    /* Initialize the server address structure with zeros */
    bzero(&serveraddr, sizeof(serveraddr));

    /* Fill in server address */
    serveraddr.sin_family = AF_INET;                /* Using IPv4 */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Using wildcard address */ 
    serveraddr.sin_port = htons(SERV_PORT);

    /* Assign the local protocol addres to socket */
    if(bind(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        fprintf(stderr, "ERROR: failed to bind\n");
        return -1;
    }

    /* Initialize the SCTP event subscribe structure with zeros */
    bzero(&events, sizeof(events));

    /* subscribe to sctp_data_io_event, which enables reception of SCTP_SNDRCV 
     * information on a per message basis and will allow the server to see the 
     * sctp_sndrcvinfo structure */
    events.sctp_data_io_event = 1;

    /* Set up socket oprtion for SCTP_EVENTS */
    if(setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events)) == -1)
    {
        fprintf(stderr, "ERROR: failed to set socket option\n");
        return -1;
    }

    /* Listen for new association */
    if(listen(sock_fd, LISTENQ) == -1)
    {
        fprintf(stderr, "ERROR: failed to listen\n");
        return -1;
    }
    for( ; ; )
    {
        len = sizeof(struct sockaddr_in);

        /* Wait for message from any remote peer */
        if((rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
                            (struct sockaddr*)&cliaddr, &len, &sri, &msg_flags)) == -1)
        {
            fprintf(stderr, "ERROR: failed to receive from remote peer");
            continue;
        }

        /* Increment stream number of the message: our server responds using the next
         * higher stream than the one on which the message was received */ 
        sri.sinfo_stream++;

        /* Check if the stream number larger than or equal to the maximum streams,
         * if it is, reset stream to 0 */
        if(sri.sinfo_stream >= NUM_STREAMS)
            sri.sinfo_stream = 0;

        /* Send back the received message using payload protocol ID, flags and modified 
         * stream number */
        if(sctp_sendmsg(sock_fd, readbuf,rd_sz, (struct sockaddr*)&cliaddr, len, 
                        sri.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0) == -1)
        {
            fprintf(stderr, "ERROR: failed to send back message\n");
            continue;
        }
    }
    /* This program runs forever until it is shut down with external signal */
}
/******************************************************************************/

