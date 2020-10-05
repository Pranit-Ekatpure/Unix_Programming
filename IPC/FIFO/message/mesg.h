/****************************************************************************
* Filename              : mesg.h
* Author                : Pranit Ekatpure
* Description           : This header file contains our mymesg structure and 
*                         related definitions.
*****************************************************************************/
#ifndef _MESG_H
#define _MESG_H

/******************************************************************************
* Includes
*******************************************************************************/
#include	<sys/types.h>	
#include	<sys/time.h>	
#include	<time.h>		
#include	<errno.h>
#include	<fcntl.h>		
#include	<limits.h>		
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	
#include	<unistd.h>
#include	<sys/wait.h>

/******************************************************************************
* Macros
*******************************************************************************/
/* want sizeof(struct mymesg) <= PIPE_BUF */
#define MAXMESGDATA (PIPE_BUF - 2*sizeof(long))
/* length of mesg_len and mesg_type */
#define MESGHDRSIZE (sizeof(struct mymesg) - MAXMESGDATA)

/******************************************************************************
* User data types
*******************************************************************************/
struct mymesg
{
    long    mesg_len;               /* bytes in mesg_data, can be 0 */
    long    mesg_type;              /* message type, must be > 0 */
    char    mesg_data[MAXMESGDATA];
};

/******************************************************************************
* Function Prototypes
*******************************************************************************/
ssize_t mesg_send(int, struct mymesg *);
ssize_t mesg_recv(int, struct mymesg *);

#endif 
/******************************************************************************/