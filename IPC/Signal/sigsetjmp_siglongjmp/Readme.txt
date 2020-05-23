Run the executable as follow;

$ ./a.out                       start process in background
starting main:
[1] 531                         the job-controll shell prints its process ID
$ kill _USR1 531                send the process SIGUSR1
starting sig_usr1: SIGUSR1
$ in sig_alrm: SIGUSR1 SIGALRM 
finishing sig_usr1: SIGUSR1
ending main:
                                just press RETURN
[1] + Done      ./a.out &


-- The original mask is restored when the signal handler returns.
-- Also, siglongjmp restores the signal mask that was saved by sigsetjmp.


If we change the program so that the calls to sigsetjmp and siglongjmp are replaced with calls to set
setjmp and longjmp, the final line of output becomes
  ending main: SIGUSR1

This means that the main function is executing with the SIGUSR1 signal blocked, after the call to setjmp