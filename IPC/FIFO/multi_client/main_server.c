/******************************************************************************
* Filename              : main_server.c
* Author                : Pranit Ekatpure
* Description           : This file contains server implementaion.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "multi_client_fifo.h"

/******************************************************************************
* Function Prototypes
*******************************************************************************/ 
void server(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for FIFO server that handles multiple clients.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int readfifo, writefifo, dummyfd, fd;
    char *ptr, buff[MAXLINE], fifoname[MAXLINE];
    pid_t pid;
    ssize_t n;

    /* create server's well-known FIFO; OK if already exists */
    if((mkfifo(SERV_FIFO, 0666) < 0) && (errno != EEXIST))
    {
        fprintf(stderr,"ERROR: can't create %s", SERV_FIFO);
        return -1;
    }
    
    /* open server's well-known FIFO for reading and writing */
    if((readfifo = open(SERV_FIFO, O_RDONLY, 0)) == -1)
    {
        fprintf(stderr,"ERROR: can't open %s for read", SERV_FIFO);
        return -1;
    }
    if((dummyfd = open(SERV_FIFO, O_WRONLY, 0)) == -1)      /* never used */
    {
        fprintf(stderr,"ERROR: can't open %s for write", SERV_FIFO);
        return -1;
    }   

    while(1)
    {
        /* read line from client */
        if((n = read(readfifo, buff, MAXLINE)) < 0)
        {
            fprintf(stderr, "ERROR: main_server, read failed\n");
            return -1;
        }
        if(buff[n - 1] == '\n')
            n--;            /* delete newline from read() */
        /* null teminate path name */
        buff[n] = '\0';     

        ptr = buff;
        /* extract the starting address of pathname */
        while(*ptr != ' ')  
            ptr++;
        ptr++;

        /* extract the pid */
        if((pid = atol(buff)) == 0)
        {
            fprintf(stderr, "ERROR: atol\n");
            return -1;
        }  
        /* construct client fifo name by using received pid from client */ 
        snprintf(fifoname, sizeof(fifoname), "fifo.%ld", (long)pid);

        /* open client fifo in write mode */
        if((writefifo = open(fifoname, O_WRONLY, 0)) < 0)
        {
            fprintf(stderr,"ERROR: cannot open: %s", fifoname);
            continue;
        }
        
        /* open file to be read */
        if((fd = open(ptr, O_RDONLY)) < 0)
        {
            /* error: must tell client */
            snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
            n = strlen(ptr);
            /* write to client about error */
            if(write(writefifo, ptr,n) != n)
            {
                fprintf(stderr,"ERROR: main_server, write failed\n");
                return -1;
            }
            /* close client fifo */
            if(close(writefifo) == -1)
                fprintf(stderr, "ERROR: close writefifo\n");
        }
        else
        {
            /* open succeeded: copy file to FIFO */
            while(1)
            {
                /* read file */
                n = read(fd, buff, MAXLINE);
                if(n < 0)
                {
                    fprintf(stderr, "ERROR: main_server, read failed\n");
                    return -1;
                }
                else if(n == 0)
                {
                    /* end of file */
                    break;
                }
                /* write content read from file to client */
                if(write(writefifo, buff, n) != n)
                {
                    fprintf(stderr,"ERROR: main_server, write failed\n");
                    return -1;
                }
            }
            /* close file */
            if(close(fd) == -1)
                fprintf(stderr, "ERROR: close fd\n");
            /* close client fifo */
            if(close(writefifo) == -1)
                fprintf(stderr, "ERROR: close writefifo\n");
        }        
    }
    /* Server runs forever until it is shut down with external signal */
    exit(0);
}
/******************************************************************************/