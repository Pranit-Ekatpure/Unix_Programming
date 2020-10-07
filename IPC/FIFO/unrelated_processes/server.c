/******************************************************************************
* Filename              : server.c
* Author                : Pranit Ekatpure
* Description           : This file contain server implementation.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "fifo.h"

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