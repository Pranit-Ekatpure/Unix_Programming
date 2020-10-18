/******************************************************************************
* Filename              : alarm.c
* Author                : Pranit Ekatpure
* Description           : This file Alarm signal example.
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void sig_alrm(int);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : main
* Description   : main function for alarm signal example.
*
* Parameters    : void
* Return value  : int
*
*******************************************************************************/
int main(void)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        fprintf(stderr, "ERROR: can't catch SIGALRM\n");
        return -1;
    }

    /* Start the timer - returns 0 as first time set */     
    printf("Remaining time_1 %d \r\n", alarm(5));  
    sleep(2);
    /* Reset the timer - returns 3 as previous set alarm remaining time value 
     * and reset timer for next 10 sec */
    printf("Remaining time_2 %d \r\n", alarm(10));  
    /* Next caught signal wakes us up */
    pause();                
    /* Turn off timer, return unslept time */
    return(alarm(0));       
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function      : sig_alarm
* Description   : Alarm signal handler.
*
* Parameters    :
*   int signo   : Signal number
* Return value  : void
*
*******************************************************************************/
static void sig_alrm(int signo)
{
    printf("Alarm signal handler invoked\n");
}
/******************************************************************************/
