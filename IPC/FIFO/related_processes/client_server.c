/******************************************************************************
* Filename              : client_server.c
* Author                : Pranit Ekatpure
* Description           : This file contain client-server implementation using
*						  two FIFOs.
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
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MAXLINE 4096

/******************************************************************************
* Macros
*******************************************************************************/
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void client(int, int), server(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function client-server implementation that uses two FIFOs.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int readfd, writefd;
    pid_t childpid;

    /* create two FIFOs; OK if they already exist */
    if((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
	{
        fprintf(stderr, "ERROR: can't create %s\n", FIFO1);
		return -1;
	}
    if((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        if(unlink(FIFO1) == -1)
			fprintf(stderr, "ERROR: unlink %s\n", FIFO1);
        fprintf(stderr, "ERROR: can't create %s", FIFO2);
		return -1;
    }

	/* fork, client runs server function and the parent runs client function */
    if((childpid = fork()) == 0)       /* child */ 
    {
		/* child: open FIFO1 for reading */
        if((readfd = open(FIFO1, O_RDONLY, 0)) == -1)
		{
			fprintf(stderr, "ERROR: child, open %s\n", FIFO1);
			return -1;
		}
		/* child: open FIFO2 for writing */
		if((writefd = open(FIFO2, O_WRONLY, 0)) == -1)
		{
			fprintf(stderr, "ERROR: child, open %s\n", FIFO2);
			return -1;
		}
        /* server in child */
        server(readfd, writefd);
        exit(0);
    }
    /* parent */
	/* parent: open FIFO1 for writing */
    if((writefd = open(FIFO1, O_WRONLY, 0)) == -1)
	{
		fprintf(stderr, "ERROR: parent, open %s\n", FIFO1);
		return -1;
	}
	/* parent: open FIFO2 for reading */
    if((readfd = open(FIFO2, O_RDONLY, 0)) == -1)
	{
		fprintf(stderr, "ERROR: parent, open %s\n", FIFO2);
		return -1;
	}
	/* client in parent */
    client(readfd, writefd);

	/* wait for child to termiante */
    if(waitpid(childpid, NULL, 0) == -1)
		fprintf(stderr, "ERROR: waipid\n");         

	/* close FIFOs */
    if(close(readfd) == -1)
		fprintf(stderr, "ERROR: close readfd\n");
    if(close(writefd) == -1)
		fprintf(stderr, "ERROR: close writefd\n");

	/* delete FIFOs */
	if(unlink(FIFO1) == -1)
		fprintf(stderr, "ERROR: unlink %s\n", FIFO1);
    if(unlink(FIFO2) == -1)
		fprintf(stderr, "ERROR: unlink %s\n", FIFO2);
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
		
	/* write pathname to FIFO channel */
	if(write(writefd, buff, len) != len)
	{
		fprintf(stderr, "ERROR: client, write to FIFO failed\n");
		return;
	}
		
	/* read from FIFO, write to standard output */
	while(1)
	{
		if((n = read(readfd, buff, MAXLINE)) < 0)
		{
			fprintf(stderr, "ERROR: client, read from FIFO failed\n");
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

	/* read pathname from FIFO channel */
	if((n = read(readfd, buff, MAXLINE)) <= 0)
	{
		fprintf(stderr, "ERROR: failed to read pathname FIFO channel\n");
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
			fprintf(stderr, "ERROR: write to FIFO failed\n");
	}
	else
	{
		/* open succeeded: copy file to FIFO channel */
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
				fprintf(stderr, "ERROR: failed to write to FIFO\n");
				return;
			}
		}
		/* close file */
		if(close(fd) == -1)
			fprintf(stderr, "ERROR: failed to close file\n");
	}
}
/******************************************************************************/