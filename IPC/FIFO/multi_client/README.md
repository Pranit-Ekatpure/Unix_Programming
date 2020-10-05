### This directory contains example of one server, multiple clients.
Server handles multiple clients. Server creates FIFO named SERV_FIFO to read clients requests. Each client create FIFO with FIFO name appended with it's PID. For example, client1 with PID 9999 will create FIFO named fifo.9999. Clients write to the SERV_FIFO its PID along with pathname of file whose content to be read. Server reads PID of client and pathname of file through SERV_FIFO. Server then reads the file, open the FIFO of requested client (fifo.* PID received from client *) in write mode and writes file's content to this FIFO. Client reads its FIFO and output it on standard output.

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
- Run first client
```
    $ ./main_client1
    Enter the file name that client with PID 10791 want to read: test.txt
    (Content of test.txt read by server will be diplayed here)
```
- Run second client
```
    $ ./main_clietn2
    Enter the file name that client with PID 10795 want to read: test.txt
    (Content of test.txt read by server will be diplayed here)
```
    (Server opens file whose pathname received from client, reads file and write to client. Client reads from server and write to standard output.)
