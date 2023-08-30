#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

void clean(int sig)
{
    wait(NULL);
    printf("clean\n");
}
int main(int argc,char **argv)
{
    // kill(atoi(argv[1]),SIGSEGV);
    // signal(SIGCHLD, clean);
    // if (fork() == 0)
    //     return 0;
    // pause();
    // sigset_t sig;
    // sigemptyset(&sig);
    // sigaddset(&sig,SIGHUP);
    // sigaddset(&sig,SIGINT);
    // sigprocmask(SIG_SETMASK,&sig,NULL);
    // printf("self:%d\n",getpid());
    signal(SIGINT,SIG_IGN);
    while (1)
    {
        
    }
    
    return 0;
}