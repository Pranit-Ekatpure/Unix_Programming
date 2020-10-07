/******************************************************************************
* Filename              : main_client.c
* Author                : Pranit Ekatpure
* Description           : This file contain client main function.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "fifo.h"

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void client(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : Stand-alone client main function.
*
* Parameters    : void
* Return value  : void
*
*******************************************************************************/
int main(void)
{
    int readfd, writefd;

	/* open FIFO1 for writing */
    if((writefd = open(FIFO1, O_WRONLY, 0)) == -1)
	{
		fprintf(stderr, "ERROR: parent, open %s\n", FIFO1);
		return -1;
	}
	/* open FIFO2 for reading */
    if((readfd = open(FIFO2, O_RDONLY, 0)) == -1)
	{
		fprintf(stderr, "ERROR: parent, open %s\n", FIFO2);
		return -1;
	}
    
	/* client call */
    client(readfd, writefd);
     
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
/******************************************************************************/