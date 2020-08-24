TCP client/server example.
Example is an echo server that performs the following steps:
    1. The client  reads a line of text from its standard input
        and writes the line to the server.
    2. The server reads the line from its network input and echoes
        the line back to the client.
    3. The client reads the echoed line and prints it on its standard
        output.

--> For each client, fork spawns a child, and the child
    handles the new client. The child closes the listening
    socket and the parent closes the connected socket.
--> The function str_echo performs the server processing
    for each client: it reads data from client and echoes   
    it back to the client.