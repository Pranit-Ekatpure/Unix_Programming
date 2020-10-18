/******************************************************************************
* Filename              : sigsuspend1.c
* Author                : Pranit Ekatpure
* Description           : This file shows use of sigsuspend to wait for a 
*                         signal handler to set a global variable. In this 
*                         program, we catch both the interrupt signal and the 
*                         quit signal, but want to wake up the main routine only 
*                         when the quit signal is caught.
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
* Variable Definitions
*******************************************************************************/
/* Set nonzero by signal handler */
volatile sig_atomic_t quitflag;         

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void sig_int(int signo);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function of example
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main()
{
    sigset_t newmask, oldmask, zeromask;

    if(signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "ERROR: signal(SIGINT) error\n");
        return -1;
    }
    if(signal(SIGQUIT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "ERROR: signal(SIGQUIT) error\n");
        return -1;
    }
    /* Initialize signal set zeromask to empty */
    if(sigemptyset(&zeromask) == -1)
    {
        fprintf(stderr, "ERROR: sigemptyset zeromask\n");
        return -1;
    }
    /* Initialize signal set newmask to empty */
    if(sigemptyset(&newmask) == -1)
    {
        fprintf(stderr, "ERROR: sigemptyset newmask\n");
        return -1;
    }
    /* Add SIGQUIT to signal set */
    if(sigaddset(&newmask, SIGQUIT) == -1)
    {
        fprintf(stderr, "ERROR: sigaddset SIGQUIT\n");
        return -1;
    }

    /* Block SIGQUIT and save curernt signal mask */
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr,"ERROR: SIG_BLOCK error\n");
        return -1;
    }

    while(quitflag == 0)
        if(sigsuspend(&zeromask) != -1)
            fprintf(stderr, "ERROR: sigsupend error\n");

    /* SIGQUIT has been caught and is now blocked; do whatever */
    quitflag = 0;

    /* Reset signal mask which unblocks SIGQUIT */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        fprintf(stderr, "ERROR: SIG_SETMASK error\n");

    exit(0);
}

/******************************************************************************
* Function      : sig_int
* Description   : Signal handler handler for SIGINT and SIGQUIT 
*
* Parameters    :
*   int signo   : Signla number
* Return value  : void
*
*******************************************************************************/
static void sig_int(int signo)          
{
    if(signo == SIGINT)
        printf("\n interrupt \n");
    else if(signo == SIGQUIT)
        quitflag = 1;                   /* set flag for main loop */
}
/******************************************************************************/
