### Exapmle of sigsuspend.
We need a way to both restore the signal mask and put the process to sleep in a single atomic operation. This feature is provided by the **sigsuspend** function.
The process is suspended until a signal is caught or until a signal occurs that terminates the process.
If a signal is caught and if the signal handler returns,then **sigsuspend** returns, and the signal mask of the process is set to its value the call to sigsuspend.
- To avoid problem accuring by situation like below:
```
    sigprocmask()       /* mask some signal */
    /* critical region */
    sigprocmask()       /* unmask previously masked signal */  
    window is open
    pause();            /* wait for signal */
```

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
- Run the example **sigsuspend** as follow
```
    $ ./sigsuspend 
    Program start: 
    In critical region: SIGINT 
    ^C                                      /* Type the interrupt character */
    In sig_int: SIGINT SIGUSR1 
    After return from sigsuspend: SIGINT 
    Program exit: 
```
This example shows the way to protect a critical region of code from a specific signal.

- Run the example **sigsuspend1** as follow
```
    $ ./sigsuspend1 
    ^c                              /* Type the interrupt character */
     interrupt
    ^C                              /* Type the interrupt character again */
     interrupt
    ^c                              /* and again */
     interrupt
    ^\                              /* Now terminate with the quit character */
    $ 
```
This example shows the use of sigsuspend to wait for a signal handler to set a global variable. We catch both the interrupt signal and the quit signal, but want to wake up the main routine only when the quit signal is caught.