### Producer-consumer example
In this example **mutex** is used to synchronize the multiple producers threads who are writing data to shared buffer simulataneously. Consumer then varify the data in the buffer. (Here consumer thread is started when all the producer threads done the job and terminates.)

If we remove the mutex locking from this example, it fails, as expected. That is the consumer detects many instances of buff[i] not equal to i. We can verify that 
the removal of the mutex locking has no effect if only one producer thread is run.

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
    count[0] = 201507
    count[1] = 206831
    count[2] = 192610
    count[3] = 198329
    count[4] = 200723
```
Where, count shows number of times each thread stored value in buffer.

