#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

#define	SERV_PORT       9877
#define IP_ADDR         "127.0.0.1"
#define	MAXLINE		    4096

int datagram_cli(FILE*, int, struct sockaddr*, socklen_t);

int main()
{
    int sockfd, return_val;
    struct sockaddr_in servaddr;

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
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, IP_ADDR, &servaddr.sin_addr) == -1)
    {
        fprintf(stderr, "ERROR: invalid address\n");
        return -1;
    }

    /* Perform client processing */
    return_val = datagram_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(return_val == -1)                    /* Error in client processing */
    {
        fprintf(stderr, "ERROR: failed client processing");
        /* Close connection to server */
        close(sockfd);
        return -1;
    }
    else if(return_val == 0)                 /* End of client processing */
    {
        fprintf(stdout, "END: end of client processing");
        /* Close connection to server */
        close(sockfd);
        exit(0);
    }
}

int datagram_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    /* Get line from standard intput */
    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        /* send line to server */
        if(sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1)
        {
            fprintf(stderr, "ERROR: failed to send to server\n");
            return -1;
        }

        /* Receive from server */
        if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1)
        {
            fprintf(stderr, "ERROR: failed to receive from server");
            return -1;
        }

        /* null terminate */
        recvline[n] = 0;
        /* output on standard output */
        if(fputs(recvline, stdout) == -1)
        {
            fprintf(stderr, "ERROR: failed to output on standard output");
            return -1;
        }
    }
    return 0;
}