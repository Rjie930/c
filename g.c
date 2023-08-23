#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(void)
{

if (fork()==0)
{
    printf("000%d\n",getpid());
    sleep(10);
    exit(123);

}
else
{
    int stat;
    wait(&stat);
        printf("susce:%d\n",WTERMSIG(stat));
}


}