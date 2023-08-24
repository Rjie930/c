#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    key_t key = ftok(".",1);
    int shmid;

    shmid = shmget(key, 1024, IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("创建SHM对象失败");
    }
    char *p = shmat(shmid , NULL,0);

    while (1)
    {
        fgets(p,50,stdin);
    }
    
    return 0;
}
