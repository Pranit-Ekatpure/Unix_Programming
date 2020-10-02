### Simple client-server communication example over SCTP socket.
Example is an echo server that performs the following steps:

1. The client reads a line of text from standard input and sends the line to the server. The line follows the form **[#] text**, where the number in bracket is the SCTP stream number on wich the text message should be sent.
2. The server receives the text message from the network, increase the stream number on which the message arrived by one, and sends the text message back to the client on this new stream number.
3. The client reads the echoed line and prints it on its standard output, displaying the stream number, stream sequence number, and text string.

### Compilation Steps
- To generate executables **sctpserver** and **sctpclient** use command:
``` 
    make all
```
Or
```
    make sctpserver
    make sctpclient
```
- To remove output files use command:
```
    make clean
```

### Running the SCTP Client-Server example
- Start the server *sctpserver* in the background
```
    $ ./sctpserver &
```
- Start the client
```
    $ ./sctpclient
```
- Send a message on stream 0
```
    [0] Hello
```
    Server echoes on stream 1: stream number greater by one on which it receives message from client.
```
    From stream: 1 seq: 0 (assoc: 0x9):[0] Hello
```
- To stop client processing type EOF character
```
    ^D
```
Then client closes the SCTP socket and shuts down SCTP association.