### Example of client-server implementation using Message Queues.
The server manages token numbers, which could be seat numbers, or something similar. It is server's job to give a token number to a client on request. There might be multiple clients requesting the server for token numbers. The server's message queue name is known to clients. Each client has its own message queue, in which server posts responses.
When a client sends a request, it sends its message queue name. The server opens client's message queue and sends its response.
The client picks up the response from its message queue and reads the token number in it.

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
- Run server in one terminal
```
    $ ./server 
```
- Run client in another terminal
```
    $ ./client
```
Sequence operation will be as follows:
```
    SERVER WINDOW                              CLIENT WINDOW
                                       
    Server started...                          Ask for a token (Press <ENTER>): <ENTER pressed>

    Server: message received                   Client: Token received from server - 1
    Server: response sent to client.           Ask for a token (Press <ENTER>): <ENTER pressed>

    Server: message received                   Client: Token received from server - 2
    Server: response sent to client.           Ask for a token (Press <ENTER>): <ENTER pressed>
```

Server runs forever until it is shut down with external signal.
Client runs until EOF charater (ctrl + D) is pressed.

