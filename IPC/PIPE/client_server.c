/******************************************************************************
* Filename              : client_server.c
* Author                : Pranit Ekatpure
* Description           : This file contain client-server implementation using
*						  pipes.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MAXLINE 4096

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void client(int, int), server(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for client-server using two pipes.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
	int pipe1[2], pipe2[2];
	pid_t childpid;
	/* create two pipes */ 
	if(pipe(pipe1) == -1)
	{
		fprintf(stderr, "ERROR: failed to create pipe1\n");
		return -1;
	}		
	if(pipe(pipe2) == -1)
	{
		fprintf(stderr, "ERROR: failed to create pipe2\n");
		return -1;
	}

	/* fork, parent calls client function and the child calls the server function */
	if((childpid = fork()) == 0)			/* child */
	{
		/* child: close write end of pipe1 */
		if(close(pipe1[1]) == -1)
		{
			fprintf(stderr, "ERROR: child, close pipe1[1]\n");
			return -1;
		}
		/* child: close read end of pipe2 */
		if(close(pipe2[0]) == -1)
		{
			fprintf(stderr, "ERROR: child, close pipe2[2]\n");
			return -1;
		}
		/* server in child process */
		server(pipe1[0], pipe2[1]);
		exit(0);
	}
	/* parent */
	/* parent: close read end of pipe1 */
	if(close(pipe1[0]) == -1)
	{
		fprintf(stderr, "ERROR: parent, close pipe1[0]\n");
		return -1;
	}
	/* parent: close write end of pipe2 */
	if(close(pipe2[1]) == -1)
	{
		fprintf(stderr, "ERROR: parent, close pipe2[1]\n");
		return -1;
	}
	/* client in parent process */
	client(pipe2[0], pipe1[1]);

	/* wiat for child */
	if(waitpid(childpid, NULL, 0) == -1)
		fprintf(stderr, "ERROR: waitpid\n");

	exit(0);
}

/******************************************************************************
* Function      : client
* Description   : Client implementation.
*
* Parameters    : 
*	int readfd	: read fd
*	int writefd	: write fd
* Return value  : void
*
*******************************************************************************/
void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;
	char buff[MAXLINE];

	printf("Client: enter the name of file to be read:");
	/* read pathname */
	if(fgets(buff, MAXLINE, stdin) == NULL && ferror(stdin))
	{
		fprintf(stderr, "ERROR: failed to read pathname from standard input\n");
		return;
	}
	/* fgets() guarntees null byte at the end */
	len = strlen(buff);				
	/* delete newline from fgets() */
	if(buff[len - 1] == '\n')
			len--;					
		
	/* write pathname to pipe */
	if(write(writefd, buff, len) != len)
	{
		fprintf(stderr, "ERROR: client, write to pipe failed\n");
		return;
	}
		
	/* read from pipe, write to standard output */
	while(1)
	{
		if((n = read(readfd, buff, MAXLINE)) < 0)
		{
			fprintf(stderr, "ERROR: client, read from pipe failed\n");
			return;
		}
		else if(n == 0)
		{
			/* end of file */
			break;
		}
		if(write(STDOUT_FILENO, buff, n) != n)
		{
			fprintf(stderr, "ERROR: client, write to standard output failed");
			return;
		}
	}
}

/******************************************************************************
* Function      : server
* Description   : Server implementation.
*
* Parameters    : 
*	int readfd	: read fd
*	int writefd	: write fd
* Return value  : void
*
*******************************************************************************/
void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE + 1];

	/* read pathname from pipe channel */
	if((n = read(readfd, buff, MAXLINE)) <= 0)
	{
		fprintf(stderr, "ERROR: failed to read pathname pipe channel\n");
		return;
	}
	/* null terminate the pathname */
	buff[n] = '\0';		
	/* open file whose pathname is received from client for reading */
	if((fd = open(buff, O_RDONLY)) < 0)
	{
		/* error: must tell client */
		snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
		n = strlen(buff);
		if(write(writefd, buff, n) != n)
			fprintf(stderr, "ERROR: write to pipe failed\n");
	}
	else
	{
		/* open succeeded: copy file to pipe channel */
		while(1)
		{
			if((n = read(fd, buff, MAXLINE)) < 0) 
			{
				fprintf(stderr, "ERROR: failed to read file\n");
				return;
			}
			else if(n == 0)
			{
				/* end of file */
				break;
			}
			if(write(writefd, buff, n) != n)
			{
				fprintf(stderr, "ERROR: failed to write to pipe\n");
				return;
			}
		}
		/* close file */
		if(close(fd) == -1)
			fprintf(stderr, "ERROR: failed to close file\n");
	}
}
/******************************************************************************/
		
		

