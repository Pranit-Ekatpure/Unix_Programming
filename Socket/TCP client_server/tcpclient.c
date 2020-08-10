#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

#define	SERV_PORT       9877
#define IP_ADDR         "127.0.0.100"
#define	MAXLINE		    4096

void str_cli(FILE* , int );

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IP_ADDR, &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    str_cli(stdin, sockfd);  

    exit(0);   
}

void str_cli(FILE* fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE,fp) != NULL)
    {
        write(sockfd, sendline, strlen(sendline));
        read(sockfd, recvline, MAXLINE);
        fputs(recvline, stdout);
    }
}