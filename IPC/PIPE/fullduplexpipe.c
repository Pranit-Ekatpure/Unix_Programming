#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int fd[2], n;
    char c;
    pid_t childpid;

    pipe(fd);                       /* assumes a full-duplex pipe */
    if((childpid = fork()) == 0)    /* child */
    { 
        sleep(3);
        if((n = read(fd[0], &c, 1)) != 1)
            printf("Child: read returned %d\n", n);
        printf("child read %c\n",c);
        write(fd[0], "c",1);
        exit(0);
    }
        /* parent */
    write(fd[1], "p",1);
    if((n = read(fd[1], &c, 1)) != 1)
        printf("parent: read returned %d\n",n);
    printf("parent read %c\n",c);
    exit(0);
}