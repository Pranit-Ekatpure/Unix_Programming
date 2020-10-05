/******************************************************************************
* Filename              : main_client2.c
* Author                : Pranit Ekatpure
* Description           : This file contains client implementation.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "multi_client_fifo.h"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for FIFO client.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    int readfifo, writefifo;
    size_t len;
    ssize_t n;
    char *ptr, fifoname[MAXLINE], buff[MAXLINE];
    pid_t pid;

    /* Create FIFO with our PID as part of name */
    pid = getpid();
    snprintf(fifoname, sizeof(fifoname),"fifo.%ld", (long)pid);
    if((mkfifo(fifoname, 0666) < 0) && (errno != EEXIST))
    {
        fprintf(stderr,"ERROR: can't create %s", fifoname);
        return -1;
    }

    /* start buffer with pid and a blank */
    snprintf(buff, sizeof(buff),"%ld", (long)pid);
    len = strlen(buff);
    ptr = buff + len;
    *ptr++ = ' ';

    /* read pathname of file from standard intput whose contents are to be read */
    printf("Enter the file name that client with PID %d want to read: ", pid);
    if((fgets(ptr, MAXLINE - len, stdin)) == NULL && ferror(stderr))
    {
        fprintf(stderr, "ERROR: fgets\n");
    }

    len = strlen(buff);         /* fgets() guarantee null byte at end */

    /* open FIFO to server and write PID and pathname to FIFO */
    if((writefifo = open(SERV_FIFO, O_WRONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: open %s\n", SERV_FIFO);
        return -1;
    }
    if(write(writefifo, buff, len) != len)
    {
        fprintf(stderr, "ERROR: wrtie to FIFO %s\n", SERV_FIFO);
        return -1;
    }

    /* now open our FIFO; blocks until server opens for writting */
    if((readfifo = open(fifoname, O_RDONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: open client fifo\n");
        return -1;
    }

    /* read from IPC, write to standard output */
    while(1)
    {
        if((n = read(readfifo, buff, MAXLINE)) < 0)
        {
            fprintf(stderr, "ERROR: read from IPC\n");
            return -1;
        }
        else if(n == 0)
        {
            /* end of read */
            break;
        }
        if(write(STDOUT_FILENO, buff, n) != n)
        {
            fprintf(stderr, "ERROR: write to standard output\n");
            return -1;
        }
    }
    /* close client fifo */
    if(close(readfifo) == -1)
        fprintf(stderr, "ERROR: close readfifo\n");
    /* delete client fifo */
    if(unlink(fifoname) == -1)
        fprintf(stderr, "ERROR: unlink fifoname\n");
    exit(0);
}
/******************************************************************************/