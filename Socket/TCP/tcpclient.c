/******************************************************************************
* Filename              : tcpclient.c
* Author                : Pranit Ekatpure
* Description           : This file conatain TCP client-server example's client
*                         implementation.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

/******************************************************************************
* Macros
*******************************************************************************/
#define	SERV_PORT       9877
#define IP_ADDR         "127.0.0.1"
#define	MAXLINE		    4096

/******************************************************************************
* Function Prototypes
*******************************************************************************/
int str_cli(FILE* , int );

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : TCP echo client: main function
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    int sockfd, return_val;
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
    return_val = str_cli(stdin, sockfd);
    if(return_val == -1)                 /* Error in client processing */
    {
        fprintf(stderr, "ERROR: failed client processing");
        /* Close connection to the server */
        close(sockfd);
        return -1;
    }
    else if(return_val == 0)             /* End of client processing */
    {
        fprintf(stdout, "END: end of client processing");
        /* Close connection to the server */
        close(sockfd);
        exit(0);
    }

    exit(0);   
}

/******************************************************************************
* Function      : str_cli
* Description   : TCP echo client: str_cli function
*
* Parameters    : 
*   FILE* fp    : file pointer
*   int sockfd  : socket fd
* Return value  : int
*
*******************************************************************************/
int str_cli(FILE* fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    int n;

    /* Get input line from standard input */
    while(fgets(sendline, MAXLINE,fp) != NULL)
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
/******************************************************************************/