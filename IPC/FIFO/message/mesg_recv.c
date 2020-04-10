#include "mesg.h"

ssize_t mesg_recv(int fd, struct mymesg *mptr)
{
    size_t len;
    ssize_t n;

        /* read message header first, to get len of data that follows */
    if((n = read(fd, mptr, MESGHDRSIZE)) == 0)
        return (0);             /* end of file */
    else if(n != MESGHDRSIZE)
        printf("message header: expected %ld, got %ld", MESGHDRSIZE, n);

    if((len = mptr->mesg_len) > 0)
        if((n = read(fd, mptr->mesg_data, len)) != len)
            printf("message data: expected %ld, got %ld", len, n);

    return (len);
}