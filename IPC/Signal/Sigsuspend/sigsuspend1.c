#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

volatile sig_atomic_t quitflag;         /* set  nonzero by signal handler */

static void sig_int(int signo)          /* one signal handler for SIGINT and SIGQUIT */
{
    if(signo == SIGINT)
        printf("\n interrupt \n");
    else if(signo == SIGINT)
        quitflag = 1;                   /* set flag for main loop */
}

int main()
{
    sigset_t newmask, oldmask, zeromask;

    if(signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal(SIGINT) error");
    if(signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal(SIGQUIT) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    // Block SIGQUIT and save curernt signal mask.
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        printf("SIG_BLOCK error");

    while(quitflag == 0)
        sigsuspend(&zeromask);

    // SIGQUIT has been caught and is now blocked; do whatever.
    quitflag = 0;

    // Reset signal mask which unblocks SIGQUIT.
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        printf("SIG_SETMASK error");

    exit(0);
}
