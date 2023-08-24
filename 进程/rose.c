/*
 * @Author: vincent
 * @Date: 2023-08-24 11:22:59
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-24 11:42:52
 * @Description: 
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include "head.h"

int main()
{
    // 创建消息队列IPC对象
    key_t key = ftok(PROJ_PATH, PROJ_ID);
    int id = msgget(key, IPC_CREAT|0666);

    // 从消息队列接收消息
    struct msgbuf buf;
    msgrcv(id, &buf, sizeof(buf), JACK2ROSE, 0);

    printf("data: %s\n", buf.data);
}