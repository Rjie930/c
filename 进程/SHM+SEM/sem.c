/*
 * @Author: vincent
 * @Date: 2023-08-24 02:04:20
 * @LastEditTime: 2023-08-24 02:25:52
 */
#include "head.h"

void sem_init(int id, int semnum, int val)
{
    union semun
    {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO */
    }a;
    a.val = val;
    semctl(id, semnum, SETVAL, a);
}

int sem_v(int id, int semnum)
{
    struct sembuf buf[1];
    bzero(&buf, sizeof(buf));

    buf[0].sem_num = semnum; 
    buf[0].sem_op  = +1;   // 增加1个资源
    buf[0].sem_flg = 0;    // 不设置额外的选项

    return semop(id, buf, 1);
}


int sem_p(int id, int semnum)
{
    struct sembuf buf[1];
    bzero(&buf, sizeof(buf));

    buf[0].sem_num = semnum; 
    buf[0].sem_op  = -1;   // 申请1个资源
    buf[0].sem_flg = 0;    // 不设置额外的选项

    return semop(id, buf, 1);
}