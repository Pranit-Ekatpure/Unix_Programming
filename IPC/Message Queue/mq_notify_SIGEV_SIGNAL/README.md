### Example: Simple Signal Notification
Generate `SIQUSR1` when message placed onto an empty queue.<br />
Establish signal handler for `SIGUSR1`. Fill in the `sigev_notify` member of the `sigevent` structure with the `SIGEV_SIGNAL` constant, which says we want a signal generated when the queue goes from empty to not-empty. We set the `sigev_signo` member to the signal that we want generated and call `mq_notify`.
