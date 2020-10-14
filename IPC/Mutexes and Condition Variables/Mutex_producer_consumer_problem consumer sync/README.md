### Producer-consumer problem
In this example **mutex** is used to synchronize the multiple producers threads who are writing data to shared buffer simulataneously. Consumer then varify the data in the buffer. 
(Here consumer thread also initiated when all the threads are created. Consumer thread is synchronized with producer threads with wait function implementation to wait for data available to read from producers.)

Mutexes are for locking and cannot be used for waiting. We must synchronize the consumer with the producer to make certian the consumer processes only data items that have already been stored by the producer. Our consume_wait function must wait until the producer have generated the ith item.

The fundamental problem is: what can we do when the desired item is not ready?
All we do is loop around again, unblocking and locking the mutex each time. This is called spinning or polling and is waste of CPU time.

We could also sleep for a short amount of time, but we do not know how long to sleep. What is needed is another type of synchronization that lets a thread (or process) sleep until some event occurs.

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
    $ ./prod_consum
    count[0] = 222323
    count[1] = 293582
    count[2] = 167545
    count[3] = 167217
    count[4] = 149333
```
Where, count shows number of times each thread stored value in buffer.
