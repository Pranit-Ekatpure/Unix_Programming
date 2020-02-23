

1. Client-server implementation using pipe.
2. Fullduplex pipe: some systems provide full-duplex pipes, SVR4's(solaris) pipe function and the socketpair function provided by many kernels.
    In the programm, the  purpose of the sleep in the child is to allow the parent to call read before the child can call read,      
    to see whether the parent reads back what it wrote.
