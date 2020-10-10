### Example: Simple Signal Notification
Generate **SIQUSR1** when message placed onto an empty queue. Establish signal handler for **SIGUSR1**. Fill in the **sigev_notify** member of the **sigevent** structure with the **SIGEV_SIGNAL** constant, which says we want a signal generated when the queue goes from empty to not-empty. We set the **sigev_signo** member to the signal that we want generated and call **mq_notify**.

### Compilation Steps
- First generate executables **mqcreate** and **mqsend** by running below commands under directory '/Message_Queue':
``` 
    make mqcreate
    make mqsend
```
- Then cd to mq_notify_SIGEV_SIGNAL and run below command:
```
    make all
```
- To remove output files from above both directory use command in each:
```
    make clean
```

### Running the example
- First run 'mqcreate'to create Message Queue in dirctory /Message_Queue in one terminal
```
    $ ./mqcreate
```
- Run 'mq_notify_SIGEV_SIGNAL' from directory /mq_notify_SIGEV_SIGNAL in another terminal
```
    $ ./mq_notify_SIGEV_SIGNAL
    It will block on event to occure
```
- Now send message to Queue from first terminal by command
```
    $ ./mqsend
```

(Signal SIGUSR1 will be generated, will envoke signal handler and message is read from Queue in handle.)

Second terminal will show following message:
```
SIGUSR1 received, read 100 bytes
```
