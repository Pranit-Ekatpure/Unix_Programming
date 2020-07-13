
/*
* client.c: Server program to demonstrate inter process
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
    char client_queue_name[64];
    mqd_t qd_server, qd_client;         // queue descriptors
   
        /* create the client queue for receiving message from server */
    sprintf(client_queue_name, "/client-server-example-%d", getpid());
    
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if((qd_client = mq_open(client_queue_name, O_RDONLY|O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        printf("Client: mq_open(client) failed");
        exit(1);
    }

    if((qd_server = mq_open(QUEUE_NAME, O_WRONLY)) == -1)
    {
        printf("Client: mq_open(server) failed...");
        exit(1);
    }

    char in_buffer[MAX_BUFFER_SIZE];

    printf("Ask for a token (Press <ENTER>):");

    char temp_buf[10];
    
    while(fgets(temp_buf, 2, stdin))
    {
            /* send message to server */
        if(mq_send(qd_server, client_queue_name, strlen(client_queue_name) + 1, 0) == -1)
        {
            printf("Client: mq_send failed...");
            continue;
        }
        
            /* receive reply from server */
        if(mq_receive(qd_client, in_buffer, MAX_BUFFER_SIZE, NULL) == -1) 
        {
            printf("Client: mq_receive failed...");
            continue;
        }

            /* display token received from server */
        printf("Client: Token received from server - %s\n\n", in_buffer);

        printf ("Ask for a token (Press ): \n");
    }
        
    if (mq_close (qd_client) == -1) {
        printf("Client: mq_close failed...");
        exit (1);
    }

    if (mq_unlink (client_queue_name) == -1) {
        printf("Client: mq_unlink failed");
        exit (1);
    }
    printf ("Client: bye\n");

    exit (0);
}

