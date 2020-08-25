### Simple client-server communication example over TCP socket.
Example is an echo server that performs the following steps: <br />
    1. The client  reads a line of text from its standard input
        and writes the line to the server. <br />
    2. The server reads the line from its network input and echoes
        the line back to the client. <br />
    3. The client reads the echoed line and prints it on its standard
        output. <br />
> ->For each client, fork spawns a child, and the child handles the new client. The child closes the listening socket and the parent closes the connected socket. <br />
> ->The function str_echo performs the server processing for each client: it reads data from client and echoes it back to the client.
