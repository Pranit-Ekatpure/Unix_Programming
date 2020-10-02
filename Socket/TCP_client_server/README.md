### Simple client-server communication example over TCP socket.
Example is an echo server that performs the following steps:

1. The client  reads a line of text from its standard input and writes the line to the server. 
2. The server reads the line from its network input and echoes the line back to the client.
3. The client reads the echoed line and prints it on its standard output.

- For each client, fork spawns a child, and the child handles the new client. The child closes the listening socket and the parent closes the connected socket.
- The function **str_echo** performs the server processing for each client: it reads data from client and echoes it back to the client.

### Compilation Steps
- To generate executables **tcpserver** and **tcpclient** use command:
``` 
    make all
```
Or
```
    make tcpserver
    make tcpclient
```
- To remove output files use command:
```
    make clean
```

### Running the TCP Client-Server example
- Start the server *tcpserver* in the background
```
    $ ./tcpserver &
```
- Start the client
```
    $ ./tcpclient
```
- Send a message to the server
```
    Hello World!
```
    Server echoes back to client.
```
    Hello World!
```
- To stop client processing type EOF character
```
    ^D
```
