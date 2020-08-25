#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define	SERV_PORT       9877
#define	MAXLINE		    4096

int datagram_echo(int, struct sockaddr*, socklen_t);

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    /* Create a socket that uses an internet IPv4 address,
     * Set the socket to be datagram based (UDP),
     * choose the default protocol */  
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create socket\n");
        return -1;
    }

    /* Initialize the server address struct with zeros */
    bzero(&servaddr, sizeof(servaddr));

    /* Fill in the server address */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    /* Assign local protocol address to the socket */
    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        fprintf(stderr, "ERROR: failed to bind\n");
        return -1;
    }

    /* Perform server processing */
    if(datagram_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) == -1)
    {
        fprintf(stderr, "ERROR: failed servr processing");
        return -1;
    }
}

int datagram_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for( ; ; )
    {
        len = clilen;
        /* Receive data from client */
        if((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len)) == -1)
        {
            fprintf(stderr, "ERROR: failed to receive from client");
            return -1;
        }

        /* Write data back to client */
        if((n = sendto(sockfd, mesg, n, 0, pcliaddr, len)) == -1)
        {
            fprintf(stderr, "ERROR: failed to receive from client");
            return -1;
        }
    }
}