### Example of client-server implementation using Message Queues.
The server manages token numbers, which could be seat numbers, or something similar. It is server's job to give a token number to a client on request. There might be multiple clients requesting the server for token numbers. The server's message queue name is known to clients. Each client has its own message queue, in which server posts responses.
When a client sends a request, it sends its message queue name. The server opens client's message queue and sends its response.
The client picks up the response from its message queue and reads the token number in it.
