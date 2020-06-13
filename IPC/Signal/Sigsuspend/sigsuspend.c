#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

static void sig_int(int);
void pr_mask(const char *);

int main()
{
    sigset_t newmask, oldmask, waitmask;
    
    pr_mask("program start: ");

    if(signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal(SIGINT) error");
    sigemptyset(&waitmask);
    sigaddset(&waitmask,SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    // Block SIGINT and save curernt signal mask.
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        printf("SIG_BLOCK error");

    // Critical region of code.
    pr_mask("in critical region: ");

    //Pause, allowing all signals except SIGUSR1.
    if(sigsuspend(&waitmask) != -1)
        printf("sigsuspend error");
    pr_mask("after return from sigsuspend: ");

    //Reset signal mask which unblocks SIGINT.
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        printf("SIG_SETMASK error");
    
    //and continue processing...
    pr_mask("program exit: ");

    exit(0);
}

static void sig_int(int signo)
{
    pr_mask("\n in sig_int: ");
}

void pr_mask(const char * str)
{
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    if(sigprocmask(0, NULL, &sigset)  < 0)
    {
        printf("sigprocmask error");
        return;
    }
    printf("%s", str);

    if(sigismember(&sigset, SIGINT))
        printf("SIGINT ");
    if(sigismember(&sigset, SIGQUIT))
        printf("SIGQUIT ");
    if(sigismember(&sigset, SIGUSR1))
        printf("SIGUSR1 ");
    if(sigismember(&sigset,SIGALRM))
        printf("SIGALRM ");
    
    printf("\n");
    errno = errno_save;
}