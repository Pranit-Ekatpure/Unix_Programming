### Semaphore solution to producer-consumer problem using memory based semphare.

Here also three different types of Semaphores are used:

1. A binary Semaphore named **mutex** protects the critical regions: inserting a data item into the buffer(for the producer) and removing a data item from the buffer(for the consumer). A binary Semaphores that is used as a mutex.
2. A counting semaphore named **nempty** counts the number of empty slots in the buffer. This semaphore is initialized to the number of slots in the buffer (NBUFF).
3. A counting semaphore named **nstored** counts the number of filled slots in the buffer. This semaphore is initialized to 0, since the buffer is initially empty.

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
```
Producer write and consumer read to circular buffer succeeds using Semaphore for synchronization.
