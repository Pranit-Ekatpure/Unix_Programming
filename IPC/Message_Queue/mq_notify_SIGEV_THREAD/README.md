### Example: notification by creating new thread to execute specified function.
Set **sigev_notify** to **SIGEV_THREAD**, which causes a new thread to be created. The function specified by the **sigev_notify_function** is called with the parameter of **sigev_value**.


### Compilation Steps
- First generate executables **mqcreate** and **mqsend** by running below commands under directory '/Message_Queue':
``` 
    make mqcreate
    make mqsend
```
- Then cd to mq_notify_SIGEV_THREAD and run below command:
```
    make all
```
- To remove output files from above both directory use command in each:
```
    make clean
```

### Running the example
- First run 'mqcreate' to create Message Queue in dirctory /Message_Queue in one terminal
```
    $ ./mqcreate
```
- Run 'mq_notify_SIGEV_THREAD' from directory /mq_notify_SIGEV_THREAD in another terminal
```
    $ ./mq_notify_SIGEV_THREAD
    It will block on event to occure
```
- Now send message to Queue from first terminal by command
```
    $ ./mqsend
```
(Thread will be created, and will read message from Queue)
Second terminal will show following message:
```
    Thread, read 100 bytes
```
