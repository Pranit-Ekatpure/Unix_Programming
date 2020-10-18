### Semaphore example with multiple producer and one consumer.

### Compilation Steps
- To generate executable use command:
``` 
    make all
```
- To remove output files use command:
```
    make clean
```

### Running the example
- Run example as follows
```
    $ ./sem_prod_consum
    count[0] = 96300
    count[1] = 100819
    count[2] = 100844
    count[3] = 104022
    count[4] = 102027
    count[5] = 98302
    count[6] = 98647
    count[7] = 98266
    count[8] = 100707
    count[9] = 100066
```
Where, count shows number of times each thread stored value in buffer.

Producer write and consumer read to circular buffer succeeds using Semaphore for synchronization.