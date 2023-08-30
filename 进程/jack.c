/*
 * @Author: vincent
 * @Date: 2023-08-24 11:22:59
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-24 11:39:36
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

    // 向消息队列发送消息
    struct msgbuf buf;
    buf.type = JACK2ROSE;
    strncpy(buf.data, "你好", 20);
    msgsnd(id, &buf, strlen(buf.data), 0);
}