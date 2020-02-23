#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLINE 4096

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

void client(int, int), server(int, int);

int main(int argc, char **argv)
{
    int readfd, writefd;
    pid_t childpid;

        /* create two FIFOs; OK if they already exist */
    if((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
        printf("can't create %s", FIFO1);
    if((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        printf("can't create %s", FIFO2);
    }
    if((childpid = fork()) == 0)        /* child */
    {
        readfd = open(FIFO1, O_RDONLY, 0);
        writefd = open(FIFO2, O_WRONLY, 0);
        
        server(readfd, writefd);
        exit(0);
    }
        /* parent */
    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);

    client(readfd, writefd);

    waitpid(childpid, NULL, 0);         /* wait for child to termiante */

    close(readfd);
    close(writefd);

    unlink(FIFO1);
    unlink(FIFO2);
    exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;

	char buff[MAXLINE];
	printf("Enter the name of file to read:");
		/* read pathname */
	fgets(buff, MAXLINE, stdin);
	len = strlen(buff);					/* fgets() guarntees null byte at the end */
	if(buff[len - 1] == '\n')
			len--;							/* delete newline from fgets() */
		
		/* write pathname to IPC channel */
	write(writefd, buff, len);
		
		/* read from IPC, write to standard output */
	while((n = read(readfd, buff, MAXLINE)) > 0)
		write(STDOUT_FILENO, buff, n);
}

void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE + 1];

		/* read pathname from IPC channel */
	if((n = read(readfd, buff, MAXLINE) == 0))
			printf("end-of-file while reading pathname\r\n");
	
	buff[n-1] = '\0';		/* null terminate the pathname */

	if((fd = open(buff, O_RDONLY)) < 0){
		/* error: must tell client */
	snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
	n = strlen(buff);
	write(writefd, buff, n);
	}else{
		/* open succeeded: copy file to IPC channel */
	while((n = read(fd, buff, MAXLINE)) > 0)
		write(writefd, buff, n);
	close(fd);
	}
}