#include "fifo.h"
 
void server(int, int);

int main(int argc, char **argv)
{
    int readfd, writefd;
    
         /* create two FIFOs; OK if they already exist */
    if((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
        printf("can't create %s", FIFO1);
    if((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        printf("can't create %s", FIFO2);
    }
    readfd = open(FIFO1, O_RDONLY, 0);
    writefd = open(FIFO2, O_WRONLY, 0);

    server(readfd, writefd);
    exit(0);
}