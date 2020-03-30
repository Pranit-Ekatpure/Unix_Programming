#include "fifo.h"
 
void server(int, int);

int main(int argc, char **argv)
{
    int readfifo, writefifo, dummyfd, fd;
    char *ptr, buff[MAXLINE], fifoname[MAXLINE];
    pid_t pid;
    ssize_t n;

        /* create server's well-known FIFO; OK if already exists */
    if((mkfifo(SERV_FIFO, 0666) < 0) && (errno != EEXIST))
        printf("can't create %s", FIFO1);
    
        /* open server's well-known FIFO for reading and writing */
    readfifo = open(SERV_FIFO, O_RDONLY, 0);
    dummyfd = open(SERV_FIFO, O_WRONLY, 0);       /* never used */
    
    while((n = read(readfifo, buff, MAXLINE)) > 0)
    {
        if(buff[n - 1] == '\n')
            n--;            /* delete newline from read() */
        buff[n] = '\0';     /* null teminate path name */

        ptr = buff;
        while(*ptr != ' ')  /* extract the starting address of pathname */
            ptr++;
        ptr++;

        pid = atol(buff);   /* extract the pid */

        snprintf(fifoname, sizeof(fifoname), "fifo.%ld", (long)pid);

        if((writefifo = open(fifoname, O_WRONLY, 0)) < 0)
        {
            printf("cannot open: %s", fifoname);
            continue;
        }
        if((fd = open(ptr, O_RDONLY)) < 0)
        {
            /* error: must tell client */
            snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
            n = strlen(ptr);
            write(writefifo, ptr,n);
            close(writefifo);
        }
        else
        {
                /* open succeeded: copy file to FIFO */
            while((n = read(fd, buff, MAXLINE)) > 0)
                write(writefifo, buff, n);
            close(fd);
            close(writefifo);
        }
        
    }
    exit(0);
}