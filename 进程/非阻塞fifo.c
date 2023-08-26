/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-08-26 10:10
 * @LastEditTime   : 2023-08-26 10:10
*******************************************************************/
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
#include <time.h>

int main(void)
{
    char buf[50];
    int fd = open("fifo", O_RDWR);

    long write_flag = fcntl(fd, F_GETFL);
    write_flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, write_flag);

    int ret, sum = 0;
    time_t curr;
    while (1)
    {
        time(&curr);
        char *p = ctime(&curr);
        // printf("%s\n", p);
        sprintf(buf, "%d,%s", getpid(), p);
        ret = write(fd, buf, 1);
        if (ret > 0)
        {
            sum += ret;
        }
        else
        {
            printf("%d\n", sum);
            break;
        }
    }
    return 0;
}
