### Producer-consumer problem
Here **condition variable** is used to wait for data to avaible in the buffer from producers and signal the consumer when data is avaible.

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
    count[0] = 198765
    count[1] = 201167
    count[2] = 197004
    count[3] = 204964
    count[4] = 198100
```
Where, count shows number of times each thread stored value in buffer.
