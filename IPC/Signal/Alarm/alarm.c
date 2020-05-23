#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_alrm(int);

int main()
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return -1;
    
    printf("remaining time_1 %d \r\n", alarm(5));  /* start the timer - returns 0 as first time set */ 
    sleep(2);
    printf("remaining time_2 %d \r\n", alarm(10));  /* resent the timer - returns 3 as previous set alarm remaining time value and reset timer for next 10 sec */

    pause();                /* next caught signal wakes us up */

    return(alarm(0));       /* turn off timer, return unslept time */
}

static void sig_alrm(int signo)
{
    printf("Alarm signal handler invoked...");
}
