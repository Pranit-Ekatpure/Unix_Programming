/******************************************************************************
* Filename              : main_client_server.c
* Author                : Pranit Ekatpure
* Description           : The client-server example using FIFOs that uses 
*                         message structure.
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "mesg.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MAXLINE 4096

/******************************************************************************
* Macros
*******************************************************************************/
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void client(int, int), server(int, int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for client-server using FIFOs which uses
*                 message structure.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    int readfd, writefd;
    pid_t childpid;

    /* create two FIFOs; OK if they already exist */
    if((mkfifo(FIFO1, 0666) < 0) && (errno != EEXIST))
    {
        fprintf(stderr,"ERROR: can't create %s\n", FIFO1);
        return -1;
    }
    if((mkfifo(FIFO2, 0666) < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        fprintf(stderr, "ERROR: can't create %s\n", FIFO2);
        return -1;
    }
    /* fork, child calls our server function and parent calls our client function*/
    if((childpid = fork()) == 0)        /* child */
    {
        /* child: open FIFO1 in read mode */
        if((readfd = open(FIFO1, O_RDONLY, 0)) == -1)
        {
            fprintf(stderr, "ERROR: failed to open %s in child\n", FIFO1);
            return -1;
        }  
        /* child: open FIFO2 in write mode */ 
        if((writefd = open(FIFO2, O_WRONLY, 0)) == -1)
        {
            fprintf(stderr, "ERROR: failed to open %s in child\n", FIFO2);
            return -1;
        }
        
        /* server in child */
        server(readfd, writefd);
        exit(0);
    }
    
    /* parent */
    /* parent: open FIFO1 in write mode */
    if((writefd = open(FIFO1, O_WRONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open %s in parent\n", FIFO1);
        return -1;
    }
    /* parent: open FIFO2 in read mode */
    if((readfd = open(FIFO2, O_RDONLY, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to open %s in parent\n", FIFO2);
        return -1;
    }

    /* client in parent */
    client(readfd, writefd);

    /* wait for child to termiante */
    if(waitpid(childpid, NULL, 0) == -1)
        fprintf(stderr, "ERROR: waitpid\n");         

    /* close FIFOs */
    if(close(readfd) == -1)
        fprintf(stderr, "ERROR: close readfd\n");
    if(close(writefd) == -1)
        fprintf(stderr, "ERROR: close writefd\n");

    /* delete fifos */
    if(unlink(FIFO1) == -1)
        fprintf(stderr, "ERROR: unlink error for %s \n", FIFO1);
    if(unlink(FIFO2) == -1)
        fprintf(stderr, "ERROR: unlink error for %s \n", FIFO2);
        
    exit(0);
}
/******************************************************************************/