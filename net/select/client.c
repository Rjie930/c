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

#define BUFSIZE 1*1024

int main(int argc, char const *argv[])  // ./tcpSnd IP
{
    // 1. 创建TCP通信端点
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1)
    {
        perror("socket()失败");
        exit(0);
    }

    // 2. 准备服务端IP:PORT
    struct sockaddr_in addr; // IPv4协议的结构体
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(50001); // short

    // 3. 连接服务器
    if(connect(sockfd, (struct sockaddr *)&addr, len) != 0)
    {
        perror("连接失败");
        exit(0);
    }
    printf("连接成功\n");

    // 文件描述符集合
    while(1)
    {
        fd_set rset, wset, eset;
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);

        // 监测sockfd和标准输入的读就绪状态
        FD_SET(sockfd, &rset);
        FD_SET(sockfd, &eset);
        FD_SET(STDIN_FILENO, &rset);

        int n = select(sockfd+1, &rset, &wset, &eset, NULL);


        char buf[BUFSIZE];
        if (FD_ISSET(sockfd, &rset)) // 对端发来了数据
        {
            bzero(buf, BUFSIZE);
            int n = recv(sockfd, buf, 8, MSG_OOB);
            if (n == 0)
                break;
            printf("收到OOB:%s\n", buf);
        }

        if (FD_ISSET(sockfd, &eset)) // 对端发来了数据
        {
            bzero(buf, BUFSIZE);
            int n = read(sockfd, buf, 100);
            if (n == 0)
                break;
            printf("收到:%s\n", buf);
        }

        if(FD_ISSET(STDIN_FILENO, &rset)) // 标准输入（键盘）有数据
        {
            bzero(buf, BUFSIZE);
            fgets(buf, BUFSIZE, stdin);

            int n = write(sockfd, buf, strlen(buf));
            if(n == -1)
                perror("write()失败");
            else
                printf("成功发出%d个字节\n", n);
        }
    }

    close(sockfd);
    return 0;
}
