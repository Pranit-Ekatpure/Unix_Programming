### This directory contains the client-server implementation using two FIFOs between two unrelated processes.
Client and server runs as two seperate processes.

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
- Run server in the background
```
    $ ./main_server &
```
- Run client
```
    $ ./main_client
    Client: enter the name of file to be read:test.txt
    (Content of test.txt read by server will be diplayed here)
```
(Server opens file whose pathname received from client, reads file and write to client. Client reads from server and write to standard output.)
