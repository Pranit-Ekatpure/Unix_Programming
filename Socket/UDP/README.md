### Simple client-server communication example over UDP socket.
Example is an echo server that performs the following steps:
1. The client reads a line of text from its standard input and writes the line to the server.
2. The server reads the line from its network input and echoes the line back to the client.
3. The client reads the echoed line and prints it on its standard output.

### Compilation Steps
- To generate executables **udpserver** and **udpclient** use command:
``` 
    make all
```
Or
```
    make udpserver
    make udpclient
```
- To remove output files use command:
```
    make clean
```

### Running the UDP Client-Server example
- Start the server *udpserver* in the background
```
    $ ./udpserver &
```
- Start the client
```
    $ ./udpclient
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