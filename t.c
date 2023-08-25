/*
 * @Author: vincent
 * @Date: 2023-08-25 09:56:25
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-25 10:15:44
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

int main(void)
{
    // 创建匿名管道
    int fd[2];
    pipe(fd);

    // 创建子进程
    pid_t ret = fork();
    if(ret > 0)
    {
        // 文件描述符重定向
        // 原来：
        // STDOUT_FILENO(1) --> 终端屏幕
        // fd[1]            --> 管道写端
        // fd[0]            --> 管道读端

        dup2(fd[1], STDOUT_FILENO); // cp file1 file2

        // 现在：
        // STDOUT_FILENO(1) --> 管道写端
        // fd[1]            --> 管道写端
        // fd[0]            --> 管道读端

        // 关闭用不着的文件描述符
        // close(fd[0]);
        // close(fd[1]);

        char *argv[] = {"ls", "/etc", NULL};
        execvp("ls", argv);
    }
    else if(ret == 0)
    {
        // 文件描述符重定向
        // 原来：
        // STDIN_FILENO(0)  --> 键盘
        // fd[1]            --> 管道写端
        // fd[0]            --> 管道读端

        dup2(fd[0], STDIN_FILENO);

        // 现在：
        // STDIN_FILENO(0)  --> 管道读端
        // fd[1]            --> 管道写端
        // fd[0]            --> 管道读端

        // 关闭用不着的文件描述符
        // close(fd[0]);
        close(fd[1]);

        execlp("wc", "wc", "-w", NULL);
    }
}