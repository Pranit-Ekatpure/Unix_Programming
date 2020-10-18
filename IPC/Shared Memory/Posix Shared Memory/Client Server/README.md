### Producer-consumer example using shared memory.
A server is started that creates a shared memory object in which messages are placed by client processes. Server prints these messages.

### Running the example
- Start **server** first
```
    $ ./shm_client_server_server
```
- Then run the **client**
```
    $ ./shm_client_server_client1
    Index = 0: pid 16570: message 0
    Index = 1: pid 16570: message 1
    Index = 2: pid 16570: message 2
    Index = 3: pid 16570: message 3
    Index = 4: pid 16570: message 4
    Index = 5: pid 16570: message 5
    . . .
    . . .
```

