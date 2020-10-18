### Example of incrementing shared counter by multiple clients.

### Running the example
- Start **server** first
```
    $ ./server
```
- Then run copies of the **client**
```
    $ ./client1 & ./client2 & ./client3 &
    [1] 12820       process IDs output by shell
    [2] 12821
    [3] 12822
    
    pid 12820: 0
    pid 12820: 1
    . . .
    pid 12821: 40
    pid 12821: 41
    . . . 
    pid 12822: 70
    pid 12822: 71
    . . . 
    pid 12821: 299          final value output 
```
Each cleint have their own copy of the pointer `ptr`, but each copy points to the same structure in shared memory in which integer counter is stored.