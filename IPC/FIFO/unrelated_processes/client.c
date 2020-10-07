/******************************************************************************
* Filename              : client.c
* Author                : Pranit Ekatpure
* Description           : This file contain client implementation.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "fifo.h"

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
/******************************************************************************/