#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define IP_ADDR         "127.0.0.1"
#define	SERV_PORT       9877
#define	MAXLINE		    512

void sctp_cli(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen);

int main()
{
    int sock_fd;
    struct sockaddr_in serveraddr;
    struct sctp_event_subscribe events;

    /* Create an SCTP one-to-many-style socket */
    if((sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create a socket\n");
        return -1;
    }

    /* Initialize the server address structure with zeros */
    bzero(&serveraddr, sizeof(serveraddr));

    /* Fill in server address */
    serveraddr.sin_family = AF_INET;                    /* Using IPv4 */
    serveraddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, IP_ADDR, &serveraddr.sin_addr) == -1)
    {
        fprintf(stderr, "ERROR: invalid address\n");
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

    /* Call echo processing function */
    sctp_cli(stdin, sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    /* Finish up: close SCTP socket, which shuts down any SCTP associations using the socket*/
    close(sock_fd);

    return 0;
}

void sctp_cli(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen)
{
    struct sockaddr_in peeraddr;
    struct sctp_sndrcvinfo sri;
    char sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    int out_sz, rd_sz;
    int msg_flags;

    /* Clear the sctp_sndrcvinfo structure */
    bzero(&sri, sizeof(sri));

    /* Process until EOF charecter typed */
    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        /* Validate input: make sure that input is of the form "[#] text" */
        if(sendline[0] != '[')
        {
            fprintf(stderr,"ERROR: line must be of the form '[streamnum] text'\n");
            continue;
        } 

        /* Translate stream number */
        sri.sinfo_stream = strtol(&sendline[1], NULL, 0);

        /* Initialize the size of data */
        out_sz = strlen(sendline);

        /* Send message */
        if(sctp_sendmsg(sock_fd, sendline,out_sz, to, tolen, 0, 0, sri.sinfo_stream, 0, 0) == -1)
        {
            fprintf(stderr, "ERROR: failed to send message\n");
            continue;
        }
        
        len = sizeof(peeraddr);
        /* Wait for the echoed message from the server */
        if((rd_sz = sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
                                (struct sockaddr*)&peeraddr, &len, &sri, &msg_flags)) == -1)
        {
            fprintf(stderr, "ERROR: failed to receive from server\n");
            continue;
        }

        /* Display returned message: the stream number, stream sequence number and the text message */
        printf("From stream: %d seq: %d (assoc: 0x%x):", sri.sinfo_stream, sri.sinfo_ssn, sri.sinfo_assoc_id);
        printf("%.*s", rd_sz, recvline);
    }
}