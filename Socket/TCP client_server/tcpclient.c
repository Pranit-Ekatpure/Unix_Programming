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

int str_cli(FILE* , int );

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    /* Create a socket that uses an internet IPv4 address,
     * Set the socket to be stream based (TCP),
     * choose the default protocol */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create the socket\n");
        return -1;
    }

    /* Initialize the server address struct with zeros */
    bzero(&servaddr, sizeof(servaddr));

    /* Fill in the server address */
    servaddr.sin_family = AF_INET;                      /* using IPv4 */
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, IP_ADDR, &servaddr.sin_addr) == -1)
    {
        fprintf(stderr, "ERROR: invalid address\n");
        return -1;
    }

    /* Connect to the server */
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        fprintf(stderr, "ERRRO: failed to connect\n");
        return -1;
    }

    /* Call client processing function */
    if(str_cli(stdin, sockfd) == -1)
    {
        fprintf(stderr, "ERROR: failed client processing");
        /* Close connection to the server */
        close(sockfd);
        return -1;
    }

    /* End of client processing */
    if(str_cli(stdin, sockfd) == 0)
    {
        fprintf(stdout, "END: end of client processing");
        /* Close connection to the server */
        close(sockfd);
        exit(0);
    }

    exit(0);   
}

int str_cli(FILE* fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    int n;

    /* Get input line from standard input */
    while (fgets(sendline, MAXLINE,fp) != NULL)
    {
        /* write line to server */
        if(write(sockfd, sendline, strlen(sendline)) == -1)
        {
            fprintf(stderr, "ERROR: failed to write to server\n");
            return -1;
        }

        /* read from server */
        if((n = read(sockfd, recvline, MAXLINE)) == -1)
        {
            fprintf(stderr, "ERROR: failed to read from server\n");
            return -1;
        }
        /* null terminate */
        recvline[n] = 0;
        /* write line to standard output */
        if(fputs(recvline, stdout) == EOF)
        {
            fprintf(stderr, "ERROR: failed to put to standard output");
            return -1;
        }
    }
    return 0;
}