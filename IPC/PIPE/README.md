# PIPE
Pipes are the original form of Unix IPC. They have no name, and can therefore be used only by related processes.

A pipe is created by the **pipe** function and provides a one-way (unidirectional) flow of data.
### This section shows example of client-server implementation using two pipes.
In this example client reads the name of file whose contents are to be read from standard input and send it to the server through first pipe. Server reads the content of file whose name received from first pipe and write it on second pipe. Client reads the content of file from second pipe and output it on standard output.

In the file `client_server.c` `main` function creates two pipes and `forks` a child. 
The client then runs in the parent process and the server runs in the child process. 
The first pipe is used to send the pathname from the client to the server, and the second pipe us used to send the contents of that file from the server to the client.

Some systems provide full-duplex pipes. Anything written to `fd[1]` will be availabe for reading by `fd[0]`, and anything written to `fd[0]` will be available for reading by `fd[1]`. `fullduplexpipe.c` tests a full-duplex pipe for two-away communication.

### Compilation Steps
- To generate executables use command:
``` 
    make all
```
- To remove output files use command:
```
    make clean
```

### Running the example
- Run **client_server** example
```
    $ ./client_server
    Client: enter the name of file to be read:india.txt
    (Content of india.txt read by server will be diplayed here)
```
(Server opens file whose pathname received from client, reads file and write to client. Client reads from server and write to standard output.)

- Run **fullduplex** example
```
    $ ./fullduplex
    (output varies with OS)
    (On Ubuntu 20.04.1 LTS below output is observed)
    ERROR: Parent, read returned -1
    Child read: p
    ERROR: Child, failed to write to pipe
