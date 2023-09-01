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

int main(int argc, char const *argv[]) // ./tcpSnd IP
{
    // 1. 创建TCP通信端点
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
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
    if (connect(sockfd, (struct sockaddr *)&addr, len) != 0)
    {
        perror("连接失败");
        exit(0);
    }
    printf("连接成功\n");

    // 5. 向服务端发送数据
    char buf[BUFSIZE];
    while (1)
    {
        bzero(buf, BUFSIZE);
        fgets(buf, BUFSIZE, stdin);
        write(sockfd, buf, strlen(buf));

        // 发送带外数据
        getchar();
        send(sockfd, "x", 1, MSG_OOB);
    }

    close(sockfd);
    return 0;
}
