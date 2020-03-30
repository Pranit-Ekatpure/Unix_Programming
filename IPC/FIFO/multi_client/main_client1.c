#include "fifo.h"

int main(int argc, char **argv)
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
        printf("can't create %s", fifoname);

        /* start buffer with pid and a blank */
    snprintf(buff, sizeof(buff),"%ld", (long)pid);
    len = strlen(buff);
    ptr = buff + len;
    *ptr++ = ' ';

        /* read pathname*/
    printf("Enter the file name that client with PID %d want to read: ", pid);
    fgets(ptr, MAXLINE - len, stdin);

    len = strlen(buff);         /* fgets() guarantee null byte at end */

        /* open FIFO to server and write PID and pathname to FIFO */
    writefifo = open(SERV_FIFO, O_WRONLY, 0);
        write(writefifo, buff, len);

        /* now open our FIFO; blocks until server opens for writting */
    readfifo = open(fifoname, O_RDONLY, 0);

        /* read from IPC, write to standard output */
    while((n = read(readfifo, buff, MAXLINE)) > 0)
        write(STDOUT_FILENO, buff, n);

    close(readfifo);
    unlink(fifoname);
    exit(0);
}