/******************************************************************************
* Filename              : client.c
* Author                : Pranit Ekatpure
* Description           : Client program to demonstrate inter process
*                         communication with POSIX message queues
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "unpipc.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define QUEUE_PERMISSIONS   0660
#define MAX_MESSAGES        10
#define MAX_MSG_SIZE        256
#define MAX_BUFFER_SIZE     MAX_MSG_SIZE + 10

/******************************************************************************
* Macros
*******************************************************************************/
#define QUEUE_NAME          "/server_queue"

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main client function for POSIX Message Queue inter process 
*                 communication demonstration.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    char client_queue_name[64];
    /* queue descriptors */
    mqd_t qd_server, qd_client;         
   
    /* construct the client queue name to be created */
    sprintf(client_queue_name, "/client-server-example-%d", getpid());
    
    struct mq_attr attr;
    /* set message queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    /* create the client queue for receiving message from server */
    if((qd_client = mq_open(client_queue_name, O_RDONLY|O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        fprintf(stderr, "ERROR:Client: mq_open(client) failed\n");
        exit(1);
    }
    /* open the server message queue for writing */
    if((qd_server = mq_open(QUEUE_NAME, O_WRONLY)) == -1)
    {
        fprintf(stderr, "ERROR: Client: mq_open(server) failed\n");
        exit(1);
    }

    char in_buffer[MAX_BUFFER_SIZE];

    printf("Ask for a token (Press <ENTER>):");

    char temp_buf[10];
    char *rptr;

    while(1)
    {
        /* read from standard input: <ENTER> */
        if((rptr = fgets(temp_buf, 2, stdin)) == NULL && ferror(stdin))
        {
            fprintf(stderr, "ERROR: fgets\n");
            exit(1);
        }
        else if(rptr == NULL)
        {
            /* end client requests */
            break;
        }
        
        /* send message to server */
        if(mq_send(qd_server, client_queue_name, strlen(client_queue_name) + 1, 0) == -1)
        {
            fprintf(stderr,"ERROR: Client: mq_send failed\n");
            continue;
        }
        
        /* receive reply from server */
        if(mq_receive(qd_client, in_buffer, MAX_BUFFER_SIZE, NULL) == -1) 
        {
            fprintf(stderr, "ERROR: Client: mq_receive failed\n");
            continue;
        }

        /* display token received from server */
        printf("Client: Token received from server - %s\n\n", in_buffer);

        printf ("Ask for a token (Press <ENTER>): ");

    }

    /* close client message queue */   
    if (mq_close (qd_client) == -1) {
        fprintf(stderr,"ERROR: Client: mq_close failed\n");
        exit (1);
    }
    /* unlink message queue */
    if (mq_unlink(client_queue_name) == -1) {
        fprintf(stderr,"ERROR: Client: mq_unlink failed");
        exit (1);
    }

    printf ("\nClient: STOP\n");
    exit (0);
}
/******************************************************************************/

