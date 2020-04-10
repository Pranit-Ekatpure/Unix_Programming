#include "mesg.h"

#define MAXLINE 4096

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

void client(int, int), server(int, int);

int main(int argc, char **argv)
{
    int readfd, writefd;
    pid_t childpid;

        /* create two FIFOs; OK if they already exist */
    if((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
        printf("can't create %s", FIFO1);
    if((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        printf("can't create %s", FIFO2);
    }
    if((childpid = fork()) == 0)        /* child */
    {
        readfd = open(FIFO1, O_RDONLY, 0);
        writefd = open(FIFO2, O_WRONLY, 0);
        
        server(readfd, writefd);
        exit(0);
    }
        /* parent */
    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);

    client(readfd, writefd);

    waitpid(childpid, NULL, 0);         /* wait for child to termiante */

    close(readfd);
    close(writefd);

    unlink(FIFO1);
    unlink(FIFO2);
    exit(0);
}