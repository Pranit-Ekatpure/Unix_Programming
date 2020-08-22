# Signals
Signals are software interrupts. Signals provide a way of handling asynchronous
events--for example, a user at terminal typing the interrupt key to stop a program.
Every signal has a name. <br />
Numerous conditions can generate a signal:
> -> The `terminal-generated signals` occur when users press certain terminal keys.
Ctrl + Z is used for suspending a process by sending it the signal SIGSTOP. While Ctrl + C is used to kill a process with the signal SIGINT. <br />
> -> `Hardware exceptions generate signals`: divide by 0, invalid memory reference, and the like. These
conditions are usually detected by the hardware, and the kernel is notified. The kernel then 
generates the appropriate signal for the process that was runnig at the time the codition occurred. 
For example, `SIGSEGV` is generated for a process that executes an invalid memory reference. <br />
> -> The `kill function` allows a process to send any signal to another process or process group. <br />
> -> The `kill command` allows us to send signals to other processes. This command is often used to terminate a runaway background process. <br />
> -> `Software conditions` can generate signals when a process should be notified of varous events.
Examples are SIGPIPE (generated when a process writes to a pipe that has no reader), and SIGALRM ( generated when an alarm clock set by the process expires).

Signals are classic examples of asynchronous events. We can tell the kernel to do one of three things when a signal occurs.
We call this the `disposition` of the signal, or the `action associated` with a signal.
> 1. `Ignore the signal`. This works for most signals, but two signals can never be ignored: SIGKILL and SIGSTOP. <br />
> 2. `Catch the signal`. To do this, we tell the kernel to call a function of ours whenever the signal occurs. In 
our function, we can do whatever we want to handle the conditon. For example, if the SIGCHLD signal is caught, it means that a child process has terminated, so the signal-catching function can call `waitpid` to fetch the child's process ID and termination status. <br />
> 3. `Let the default action apply`. Every signal has a default action viz ignore, temrminate the process. Default action for most of signal is to terminate the process.
