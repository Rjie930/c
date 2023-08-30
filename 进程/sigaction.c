#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <pwd.h>
#include <math.h>
void f(int sig, siginfo_t *info, void *arg)
{
    printf("recive sig:%d\n", sig);

    //额外信息
    printf("额外信息:%d\n",info->si_int);
    printf("src pid:%s\n",getpwuid(info->si_uid)->pw_name);
    
}
int main(int argc, char const *argv[])
{
    printf("action pid:%d\n",getpid());
    // 注册函数
    struct sigaction act;
    bzero(&act, sizeof(act));

    act.sa_sigaction = f;
    act.sa_flags |= SA_SIGINFO;

    struct sigaction oldact;
    sigaction(SIGHUP, &act, &oldact);

    pause();
    return 0;
}
