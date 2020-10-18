# Threads
### This section shows simple example on threads.

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
- Run the examples as follow
```
    $ ./simple_thread
    Main thread: pid 27723 tid 140711607867200 (0x7ff9f9671740)
    New thread:  pid 27723 tid 140711607863040 (0x7ff9f9670700)

    $ ./thread_cancellation
    Thread1() got a read lock
    Thread2() trying to obtain a write lock
```