#include "mesg.h"

void client(int readfd, int writefd)
{
    size_t len;
    ssize_t n;
    struct mymesg mesg;

        /* read pathname */
    printf("file to read by server: ");
    fgets(mesg.mesg_data, MAXMESGDATA, stdin);
    len = strlen(mesg.mesg_data);
    if(mesg.mesg_data[len - 1] == '\n')
        len--;              /* delete newline from fgets() */
    mesg.mesg_len = len;
    mesg.mesg_type = 1;

        /* write pathname to IPC channel */
    mesg_send(writefd, &mesg);

        /* read from IPC, write to standard output */
    while((n = mesg_recv(readfd, &mesg)) > 0)
        write(STDOUT_FILENO, mesg.mesg_data, n);
}