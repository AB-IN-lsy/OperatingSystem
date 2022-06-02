#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(void)
{
    int i = 0;
    pid_t pid;
    for (i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0)   //若为子进程则直接break退出
            break;
    }
 
    if (i < 5) {  //子进程部分 
        sleep(i);
        printf("I'am %d child , pid = %u\n", i+1, getpid());
    } 
    else{     //父进程
        sleep(i);
        printf("I'm parent\n");
    }
    return 0;
 }

