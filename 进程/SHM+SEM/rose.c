/*
 * @Author: vincent
 * @Date: 2023-08-24 11:22:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-24 02:26:18
 * @Description: 
 */

#include "head.h"

int shm_id;
int sem_id;

void cleanIPC(int sig)
{
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, NULL);
    exit(0);
}

int main()
{
    // 按ctrl+c时，删除IPC对象
    signal(SIGINT, cleanIPC);

    // 创建共享内存、信号量组IPC对象的key
    key_t shm_key = ftok(PROJ_PATH, PROJ_ID_SHM);
    key_t sem_key = ftok(PROJ_PATH, PROJ_ID_SEM);
    char *p;

    // 创建共享内存IPC对象
    shm_id = shmget(shm_key, 100, IPC_CREAT|IPC_EXCL|0666);
    if(shm_id == -1 && errno == EEXIST)
    {
        shm_id = shmget(shm_key, 100, 0666); // 直接打开IPC对象

        // 将共享内存映射到本进程空间
        p = shmat(shm_id, NULL /*此参数永远为NULL*/, 0);
        if (p == (void *)-1)
        {
            perror("shmat() 失败");
        }
    }
    else if(shm_id >= 0) // 共享内存是我创建的，马上初始化
    {
        // 将共享内存映射到本进程空间
        p = shmat(shm_id, NULL /*此参数永远为NULL*/, 0);
        if (p == (void *)-1)
        {
            perror("shmat() 失败");
        }

        // 清空共享内存
        bzero(p, 100);
    }

    // 创建信号量组
    sem_id = semget(sem_key, 1, IPC_CREAT|IPC_EXCL|0666);
    if(sem_id == -1 && errno == EEXIST)
    {
        sem_id = semget(sem_key, 1, 0666);
    }
    else if(sem_id >= 0)
    {
        // 初始化信号量组中的元素
        // 将信号量组sem_id中的代表数据（DATA）的信号量元素的初始值设置为0
        sem_init(sem_id, DATA, 0); 
    }

    // 从共享内存读出数据
    while(1)
    {
        // 对信号组sem_id中的DATA元素进行P操作
        // 返回值为-1时，代表信号量已被删除
        if(sem_p(sem_id, DATA) == -1)
            break;

        printf("共享内存: %s", p);
    }
}