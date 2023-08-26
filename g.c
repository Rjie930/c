#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
    int pid = getpid();
    printf("pid:%d\n",pid);

    int recv_pid;
    scanf("%d", &recv_pid);

    key_t key = ftok(".",1);
    int shmid;
    shmid = shmget(key, 1024, IPC_CREAT|0666);
    if (shmid < 0)
    {
        perror("");
    }
    char *p = shmat(shmid, NULL, 0);

    while (1)
    {
        fgets(p,50,stdin);
        kill(recv_pid,SIGUSR1);
    }
    
    return 0;
}

