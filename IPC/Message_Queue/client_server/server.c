/******************************************************************************
* Filename              : client.c
* Author                : Pranit Ekatpure
* Description           : Server program to demonstrate inter process
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
* Description   : main server function for POSIX Message Queue inter process 
*                 communication demonstration.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    /* queue descriptors */
    mqd_t qd_server, qd_client;  
    /* next token to be given to client */       
    long token_number = 1;              

    printf("Server started...\r\n");

    struct mq_attr attr;
    /* set message queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    /* create the server queue */
    if((qd_server = mq_open(QUEUE_NAME, O_RDONLY|O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        fprintf(stderr,"ERROR: Server: mq_open failed\n");
        exit(1);
    }

    char in_buffer[MAX_BUFFER_SIZE];
    char out_buffer[MAX_BUFFER_SIZE];

    while(1)
    {
        /* get the oldest message with highest priority */
        if(mq_receive(qd_server, in_buffer, MAX_BUFFER_SIZE, NULL) == -1)
        {
            fprintf(stderr,"ERROR: Server: mq_receive failed\n");
            exit(1);
        }
        
        printf("Server: message received\n");

        /* open client queue and send reply message to client */
        if((qd_client = mq_open(in_buffer, O_WRONLY)) == -1)
        {
            fprintf(stderr, "ERROR: Client: mq_open failed\n");
            continue;
        }

        sprintf(out_buffer, "%ld", token_number);

        if(mq_send(qd_client, out_buffer, strlen(out_buffer) + 1, 0) == -1)
        {
            fprintf(stderr,"ERROR: Server not able to send message to client\n");
            continue;
        }
        
        printf("Server: response sent to client.\n");
        token_number++;
    }
    /* Server runs forever until it is shut down with external signal */
}
/******************************************************************************/
