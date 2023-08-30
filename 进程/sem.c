#include "head.h"
#include <sys/sem.h>

void sem_init(int id, int semnum, int val)
{
    union semun
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } a;
    a.val = val;
    semctl(id, semnum, SETVAL, a);
}

int sem_v(int id, int semnum)
{
    struct sembuf buf[1];
    bzero(&buf, sizeof(buf));

    buf[0].sem_num = semnum;
    buf[0].sem_op = +1;
    buf[0].sem_flg = 0;

    return semop(id, buf, 1);
}

int sem_p(int id, int semnum)
{
    struct sembuf buf[1];
    bzero(&buf, sizeof(buf));

    buf[0].sem_num = semnum;
    buf[0].sem_op = -1;
    buf[0].sem_flg = 0;

    return semop(id, buf, 1);
}