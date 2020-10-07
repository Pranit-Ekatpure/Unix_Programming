### This directory contains the client-server implementation using two FIFOs between two related processes.
Client runs in the parent process and server runs in the child process.

### Compilation Steps
- To generate executable **client_server** use command:
``` 
    make all
```
- To remove output files use command:
```
    make clean
```

### Running the example
- Run example as follows
```
    $ ./client_server
    Client: enter the name of file to be read:test.txt

    (Content of test.txt read by server will be diplayed here)
```
(Server opens file whose pathname received from client, reads file and write to client. Client reads from server and write to standard output.)