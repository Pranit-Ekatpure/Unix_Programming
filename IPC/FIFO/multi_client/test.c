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

int main()
{
    char str[] = "12345ab";
    pid_t pid;
    pid = atol(str);
    printf("%d\r\n",pid);
    char *ptr;
    ptr = strchr(str, ' ');
    printf("%s",ptr);

    return 0;    
}