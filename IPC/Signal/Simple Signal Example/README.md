### Simple program to catch SIGUSR1 and SIGUSR2

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
- Run the example as follow
```
    $ ./signal &            start process in background
    [1] 3288                job-control shell prints job number and process ID
    $ kill -USR1 3188       send it SIGUSR1
    Received SIGUSR1
    $ kill -USR2 3188       send it SIGUSR2
    Received SIGUSR2
    $ kill 3188             now send it SIGTERM
    [1]+ Terminated         ./signal 
```