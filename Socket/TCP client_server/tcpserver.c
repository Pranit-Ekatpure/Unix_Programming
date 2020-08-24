#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define	SERV_PORT       9877
#define	LISTENQ		    1024
#define	MAXLINE		    4096

void str_echo(int sockfd);

int main()
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    /* Create a socket that uses an internet IPv4 address,
     * Set the socket to be stream based (TCP),
     * choose the default protocol */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed ot create the socket\n");
        return -1;
    }

    /* Initialize the server address struct with zeros */
    bzero(&servaddr, sizeof(servaddr));

    /* Fill in the server address */
    servaddr.sin_family = AF_INET;                  /* using IPv4 */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    /* Assign local protocol address to the socket */
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        fprintf(stderr, "ERROR: failed to bind\n");
        return -1;
    }

    /* Listen for a new connection */
    if(listen(listenfd, LISTENQ) == -1)
    {
        fprintf(stderr, "ERROR: failed to listen\n");
        return -1;
    }

    for( ; ; )
    {
        clilen = sizeof(cliaddr);

        /* Accept connections */
        if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) == -1)
        {
            fprintf(stderr, "ERROR: failed to accept connection\n");
            return -1;
        }

        /* Fork child to handle each client connection */
        if((childpid = fork()) == 0) /* child process */
        {
            /* close listening socket in child */
            close(listenfd);   
            /* process the client request */ 
            str_echo(connfd);   
            exit(0);
        }
        /* close connected socket in parent*/
        close(connfd);  
    }
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];
    /* Read from client */
    while((n = read(sockfd, buf, MAXLINE)) > 0)
        write(sockfd, buf, n); /* Write backt to the client */
}