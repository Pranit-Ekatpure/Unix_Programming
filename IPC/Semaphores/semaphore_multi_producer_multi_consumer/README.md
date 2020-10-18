### Semaphore example with mutliple producer and multiple consumer.

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
    producer count[0] = 101758
    producer count[1] = 98131
    producer count[2] = 98050
    producer count[3] = 100866
    producer count[4] = 99433
    producer count[5] = 100591
    producer count[6] = 97730
    producer count[7] = 104251
    producer count[8] = 100032
    producer count[9] = 99158

    consumer count[0] = 102359
    consumer count[1] = 98819
    consumer count[2] = 98334
    consumer count[3] = 97716
    consumer count[4] = 100283
    consumer count[5] = 97584
    consumer count[6] = 100182
    consumer count[7] = 103682
    consumer count[8] = 102065
    consumer count[9] = 98976
```
Where, count shows number of times each thread stored value in buffer.

Producer write and consumer read to circular buffer succeeds using Semaphore for synchronization.
