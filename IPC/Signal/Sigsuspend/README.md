### Exapmle of sigsuspend.
We need a way to both restore the signal mask and put the process to sleep in a single atomic operation. This feature is provided by the ` sigsuspend` function.
The process is suspended until a signal is caught or until a signal occurs that terminates the process.
If a signal is caught and if the signal handler returns,then `sigsuspend` returns, and the signal mask of the process is set to its value the call to sigsuspend.
> To avoid problem accuring by situation like below:
> 
> sigprocmask() // mask some signal  <br />
> // critical region  <br />
> sigprocmask() // unmask previously masked signal  <br />
> // window is open  <br />
> pause(); // wait for signal  <br />
