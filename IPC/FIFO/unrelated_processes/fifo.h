/****************************************************************************
* Filename              : fifo.h
* Author                : Pranit Ekatpure
* Description           : This header file contain includes and macros for
*                         multi_client example
*****************************************************************************/
#ifndef _FIFO_H
#define _FIFO_H
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MAXLINE 4096

/******************************************************************************
* Macros
*******************************************************************************/
#define FIFO1 "fifo.1"
#define FIFO2 "fifo.2"

#endif
/******************************************************************************/

