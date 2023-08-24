/*
 * @Author: vincent
 * @Date: 2023-08-24 11:28:45
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-24 02:20:08
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

#define PROJ_PATH "."
#define PROJ_ID_SHM   1
#define PROJ_ID_SEM   2

#define DATA 0 // 第0个信号量元素代表数据


void sem_init(int id, int semnum, int val);
int sem_v(int id, int semnum);
int sem_p(int id, int semnum);