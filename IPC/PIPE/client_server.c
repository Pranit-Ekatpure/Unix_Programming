#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAXLINE 4096
void client(int, int), server(int, int);

int main(int argc, char *argv[])
{
	int pipe1[2], pipe2[2];
	pid_t childpid;

	pipe(pipe1);		/* create two pipes */ 
	pipe(pipe2);

	if((childpid = fork()) == 0){		/* child */
		close(pipe1[1]);
		close(pipe2[0]);

		server(pipe1[0], pipe2[1]);
		exit(0);
	}
		/* parent */
	close(pipe1[0]);
	close(pipe2[1]);

	client(pipe2[0], pipe1[1]);

	waitpid(childpid,NULL,0);
	exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;

	char buff[MAXLINE];

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
		
		

