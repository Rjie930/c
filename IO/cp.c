/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-07-31 19:47
 * @LastEditTime   : 2023-07-31 19:47
*******************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

void main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,777);

    char *buf = calloc(10, 1024);

    while (1)
    {
        bzero(buf, 10 * 1024);
        int n = read(fd, buf, 10 * 1024);

        if (n > 0)
        {
            int total = 0;
            while (n > 0)
            {
                int m = write(fd2, buf + total, n);

                if (m == -1)
                {
                    perror("写入数据失败");
                    goto err;
                }
                total += m;
                n -= m;
            }
        }
        else if (n == 0)
        {
            printf("读完了");
            break;
        }
        else
        {
            perror("遇到错误");
            break;
        }
    }
    return 0;
err:
    close(fd);
    close(fd2);
}