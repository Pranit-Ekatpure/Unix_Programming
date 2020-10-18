### Semaphore example of producer-consumer with multiple buffers.

Here producer reads the file and write to shared buffers. Consumer writes buffer data to standard output. Using multiple buffers increases file reading speed.

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
    (Content of file sem_prod_consum.c will be displayed here.)
```
Producer read the file and stores read data in buffers, consumer writes the buffer data to standard output.
