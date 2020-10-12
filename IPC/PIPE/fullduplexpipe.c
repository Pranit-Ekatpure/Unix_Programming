/******************************************************************************
* Filename              : fullduplexpipe.c
* Author                : Pranit Ekatpure
* Description           : This file shows full-duplex pipe used for two-way 
*                         communication.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function that shows full-duplex pipe for two-way
*                 communication.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    int fd[2], n;
    char c;
    pid_t childpid;

    /* Creat a pipe: assumes a full-duplex pipe */
    if(pipe(fd) == -1)
    {
        fprintf(stderr, "ERROR: failed to create pipe\n");
        return -1;
    }
    
    /* fork, The parent writes the character p to the pipe, and then reads a
     * character from the pipe. The child sleeps for 3 seconds, reads a character
     * from the pipe, and then writes the character c to the pipe. */
    if((childpid = fork()) == 0)    /* child */
    { 
        sleep(5);
        if((n = read(fd[0], &c, 1)) != 1)
        {
            fprintf(stderr,"ERROR: Child, read returned %d\n", n);
            return -1;
        }
        printf("Child read: %c\n", c);
        if(write(fd[0], "c", 1) != 1)
        {
            fprintf(stderr, "ERROR: Child, failed to write to pipe\n");
            return-1;
        }
        exit(0);
    }
    /* parent */
    if(write(fd[1], "p", 1) != 1)
    {
        fprintf(stderr, "ERROR: Parent, failed to write to pipe\n");
        goto end;
    }
    if((n = read(fd[1], &c, 1)) != 1)
    {
        fprintf(stderr, "ERROR: Parent, read returned %d\n", n);
        goto end;
    }
    printf("Parent read %c\n", c);
    /* Wait for child */
end:
    if(waitpid(childpid, NULL, 0) == -1)
        fprintf(stderr, "ERROR: waitpid\n");
    exit(0);
}
/******************************************************************************/