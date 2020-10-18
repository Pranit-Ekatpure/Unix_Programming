/******************************************************************************
* Filename              : signal.c
* Author                : Pranit Ekatpure
* Description           : This file contain simple program to catch SIGUSR1
*                         and SIGUSR2.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* One handler for both signals */
static void sig_usr(int);       

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function to catch SIGUSR1 and SIGUSR2.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
        fprintf(stderr, "ERROR: can't catch SIGUSR1\n");
    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
        fprintf(stderr, "ERROR: can't catch SIGUSR2\n");
    
    for( ; ; )
        pause();
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : sig_usr
* Description   : Signal handler function for SIGUSR1 and SIGUSR2.
*
* Parameters    :
*   int signo   : signal number
* Return value  : void
*
*******************************************************************************/
static void sig_usr(int signo)     
{
    if(signo == SIGUSR1)
        printf("Received SIGUSR1\r\n");
    else if(signo == SIGUSR2)
        printf("Received SIGUSR2\r\n");
    else 
        printf("Received signal %d \n", signo);
    
}
/******************************************************************************/