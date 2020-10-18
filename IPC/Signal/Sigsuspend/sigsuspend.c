/******************************************************************************
* Filename              : sigsuspend.c
* Author                : Pranit Ekatpure
* Description           : This file contain example of way to protect a critical 
*                         region of code from a specific signal.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void sig_int(int);
void pr_mask(const char *);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for program protecting a critical region from 
*                 a signal.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    sigset_t newmask, oldmask, waitmask;
    
    pr_mask("Program start: ");

    if(signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr,"ERROR: Signal(SIGINT) error\n");
        return -1;
    }
    /* Initialize signal set to empty */
    if(sigemptyset(&waitmask) == -1)
    {
        fprintf(stderr, "ERROR: sigemptyset waitmask\n");
        return -1;
    }
    /* Add SIGUSR1 to signal set */
    if(sigaddset(&waitmask,SIGUSR1) == -1)
    {
        fprintf(stderr, "ERROR: sigaddset SIGUSR1\n");
        return -1;
    }
    /* Initialize signal set to empty */
    if(sigemptyset(&newmask) == -1)
    {
        fprintf(stderr, "ERROR: sigemptyset newmask\n");
        return -1;
    }
    /* Add SIGINT to signal set */
    if(sigaddset(&newmask, SIGINT) == -1)
    {
        fprintf(stderr, "ERROR: sigaddset SIGINT\n");
        return -1;
    }

    /* Block SIGINT and save curernt signal mask */
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr, "ERROR: SIG_BLOCK error\n");
        return -1;
    }

    /* Critical region of code */
    pr_mask("In critical region: ");

    /* Pause, allowing all signals except SIGUSR1 */
    if(sigsuspend(&waitmask) != -1)
        fprintf(stderr, "ERROR: Sigsuspend error\n");
    pr_mask("After return from sigsuspend: ");

    /* Reset signal mask which unblocks SIGINT. */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        fprintf(stderr,"ERROR: SIG_SETMASK error");
    
    /* and continue processing */
    pr_mask("Program exit: ");

    exit(0);
}

/******************************************************************************
* Function      : sig_int
* Description   : Signal handler for SIGINT.
*
* Parameters    :
*   int signo   : Signla number
* Return value  : void
*
*******************************************************************************/
static void sig_int(int signo)
{
    pr_mask("\nIn sig_int: ");
}

/******************************************************************************
* Function      : pr_mask
* Description   : A function that prints the names of the signals in the signal 
*                 mask of the calling process.
*
* Parameters    :
*   const char *str : Pointer to character string
* Return value  : void
*
*******************************************************************************/
void pr_mask(const char * str)
{
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    if(sigprocmask(0, NULL, &sigset)  < 0)
    {
        fprintf(stderr, "ERROR: sigprocmask error");
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
/******************************************************************************/