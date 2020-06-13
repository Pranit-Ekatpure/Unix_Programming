->  sigsuspend.c shows the correct way to protect a critical region of code from a 
    specific signal.
    - running program produces the following output:
        $ ./a.output    
        program  start:
        in critical region: SIGINT
        ^c                              type the interrupt character
        in sig_int: SIGINT SIGUSR1
        after return from sigsuspend: SIGINT
        program exit:

->  sigsuspend1.c shows use of sigsuspend to wait for a signal handler to set a global variable. We catch
    both the interrupt signal and the quit signal, but want to wake up the main routine only when the quit
    signal is caught.
    - sample output from this program is 
        $ ./a.output
        ^c                              type the interrupt character
        interrupt
        ^C                              type the interrupt character again
        interrupt
        ^c                              and again
        interrupt
        ^\                              now terminate with the quit character