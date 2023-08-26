#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>

void f(int sig);
char *p;

int main(int argc, char const *argv[])
{
    int pid = getpid();
    printf("pid:%d\n", pid);
    
    key_t key = ftok(".", 1);
    int shmid;

    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("创建SHM对象失败");
    }
    p = shmat(shmid, NULL, 0);

    signal(SIGUSR1, f);
    while (1)
        pause();
    return 0;
}
void f(int sig)
{

    printf("共享内存:%s\n", p);
}