# PIPE
Pipes are the original form of Unix IPC. They have no name, and can therefore be used only by related processes.<br />
A pipe is created by the `pipe` function and provides a one-way (unidirectional) flow of data.
### This section shows example of client-server implementation using two pipes.
In this example client reads the name of file whose contents are to be read from standard input and send it to the server through first pipe. Server reads the content of file whose name received from first pipe and write it on second pipe. Client reads the content of file from second pipe and output it on standard output.<br />
In the file`client_server.c` `main` function creates two pipes and `forks` a child. 
The client then runs in the parent process and the server runs in the child process. 
The first pipe is used to send the pathname from the client to the server, and the second pipe us used to send the contents of that file from the server to the client.<br />
<br />
Some systems provide full-duplex pipes. Anything written to `fd[1]` will be availabe for reading by `fd[0]`, and anything written to `fd[0]` will be available for reading by `fd[1]`.
`fullduplexpipe.c` tests a full-duplex pipe for two-away communication.
