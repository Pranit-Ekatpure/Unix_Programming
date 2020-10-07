/******************************************************************************
* Filename              : server.c
* Author                : Pranit Ekatpure
* Description           : This file contain server main function.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "fifo.h"

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void server(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : Stand-alone server main function.
*
* Parameters    : void
* Return value  : void
*
*******************************************************************************/

int main(int argc, char **argv)
{
    int readfd, writefd;

    /* create two FIFOs; OK if they already exist */
    if ((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
    {
        fprintf(stderr, "ERROR: can't create %s\n", FIFO1);
        return -1;
    }
    if ((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        if (unlink(FIFO1) == -1)
            fprintf(stderr, "ERROR: unlink %s\n", FIFO1);
        fprintf(stderr, "ERROR: can't create %s", FIFO2);
        return -1;
    }

    /* open FIFO1 for reading */
    if ((readfd = open(FIFO1, O_RDONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: child, open %s\n", FIFO1);
        return -1;
    }
    /* open FIFO2 for writing */
    if ((writefd = open(FIFO2, O_WRONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: child, open %s\n", FIFO2);
        return -1;
    }

    /* server call */
    server(readfd, writefd);
    exit(0);
}
/******************************************************************************/