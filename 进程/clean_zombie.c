#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
void clean(int sig)
{
    wait(NULL);
    printf("clean\n");
}
int main()
{
    signal(SIGCHLD, clean);
    if (fork() == 0)
        return 0;
    pause();
    return 0;
}