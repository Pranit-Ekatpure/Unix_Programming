
/*
* server.c: Server program to demonstrate inter process
*           communication with POSIX message queues
*/

#include "unpipc.h"

#define QUEUE_NAME          "/client-server-example"
#define QUEUE_PERMISSIONS   0660
#define MAX_MESSAGES        10
#define MAX_MSG_SIZE        256
#define MAX_BUFFER_SIZE     MAX_MSG_SIZE + 10

int main()
{
    mqd_t qd_server, qd_client;         // queue descriptors
    long token_number = 1;              // next token to be given to client

    printf("Server started...\r\n");

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if((qd_server = mq_open(QUEUE_NAME, O_RDONLY|O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        printf("Server: mq_open failed");
        exit(1);
    }

    char in_buffer[MAX_BUFFER_SIZE];
    char out_buffer[MAX_BUFFER_SIZE];

    while(1)
    {
            /* get the oldest message with highest priority */
        if(mq_receive(qd_server, in_buffer, MAX_BUFFER_SIZE, NULL) == -1)
        {
            printf("Server: mq_receive failed...");
            exit(1);
        }
        
        printf("Server: message received.\n");

            /* send reply message to client */
        if((qd_client = mq_open(in_buffer, O_WRONLY)) == -1)
        {
            printf("Client: mq_open failed...");
            continue;
        }

        sprintf(out_buffer, "%ld", token_number);

        if(mq_send(qd_client, out_buffer, strlen(out_buffer) + 1, 0) == -1)
        {
            printf("Server not able to send message to client");
            continue;
        }
        
        printf("Server: response sent to client.\n");
        token_number++;
    }
}

