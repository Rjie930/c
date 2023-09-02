#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1 * 1024
#define MAXCLI 200

int lastCli = -1; // 最后一个用户连接套接字存放的数组下标

int main(int argc, char const *argv[])
{
    // 1. 创建TCP套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定地址
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.16.133");
    addr.sin_port = htons(50001);
    bind(sockfd, (struct sockaddr *)&addr, len);

    // 3. 设定监听状态
    listen(sockfd, 3);

    int fds[MAXCLI];
    bzero(fds, MAXCLI);

    // 4. 等待对方连接
    int maxfd = sockfd;
    char buf[100];
    while (1)
    {
        fd_set rset;
        FD_ZERO(&rset);

        // 4.1 将监听套接字放入读就绪集合
        FD_SET(sockfd, &rset);

        // 4.2 将所有的连接套接字放入读就绪集合
        for (int i = 0; i <= lastCli; i++)
        {
            if (fds[i] > 0)
                FD_SET(fds[i], &rset);
            maxfd = maxfd > fds[i] ? maxfd : fds[i];
        }

        // 4.3 多路监听
        select(maxfd + 1, &rset, NULL, NULL, NULL);

        // 4.4 判定究竟发生了什么事情
        if (FD_ISSET(sockfd, &rset))
        {
            fds[++lastCli] = accept(sockfd, NULL, NULL);
        }

        for (int i = 0; i <= lastCli; i++)
        {
            if (FD_ISSET(fds[i], &rset))
            {
                // 将客户端数据反弹回去给他
                bzero(buf, 100);
                int n = read(fds[i], buf, 100);
                if (n == 0)
                {
                    fds[i] = 0;
                    continue;
                }
                printf("收到客户端消息：%s\n", buf);
                write(fds[i], buf, n);
                send(fds[i], "OOO_BBB", 8, MSG_OOB);
            }
        }
    }

    return 0;
}
