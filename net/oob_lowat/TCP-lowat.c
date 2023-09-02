
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int connfd;

void recvOOB(int sig)
{
    fprintf(stderr, "收到信号:%d\n", sig);
    char oob;
    recv(connfd, &oob, 1, MSG_OOB);
    printf("收到紧急数据：%c\n", oob);
}

int main(int argc, char **argv)
{
    // TCP
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int on = 1;
    // 设置端口复用
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rcvLowat = 0;
    int len = sizeof(rcvLowat);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &rcvLowat, &len);
    printf("默认接收缓冲区水位线: %d\n", rcvLowat); // 1

    printf("请输入你要设定的大小: ");
    scanf("%d", &rcvLowat);
    len = sizeof(rcvLowat);
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &rcvLowat, len) != 0)
    {
        perror("setsockopt() 失败");
        exit(0);
    }

    rcvLowat = 0;
    len = sizeof(rcvLowat);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &rcvLowat, &len);
    printf("目前接收缓冲区水位线: %d\n", rcvLowat);


    // 1. 准备绑定的地址结构体
    struct sockaddr_in addr;
    socklen_t socklen = sizeof(addr);
    bzero(&addr, socklen);

    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr.s_addr = inet_addr("192.168.16.133");
    addr.sin_port = htons(50001);

    // 2. 绑定地址
    if(bind(sockfd, (struct sockaddr *)&addr, socklen) !=0 )
    {
        perror("绑定失败");
        exit(0);
    }

    // 3. 设定监听状态
    listen(sockfd, 2);

    // 4. 等待连接
    connfd = accept(sockfd, NULL, NULL);

    // 准备好OOB数据触发信号函数
    signal(SIGURG, recvOOB);
    fcntl(connfd, F_SETOWN, getpid());  // 指定信号的接收者（属主）


    // 5. 接收数据
    char buf[10];
    while(1)
    {
        bzero(buf, 10);
        int n = read(connfd, buf, 10);
        if(n < 0)
        {
            perror("读取对端数据出错");
            exit(0);
        }
        else if(n == 0)
        {
            printf("对端已关闭\n");
            exit(0);
        }
        printf("%s", buf);
    }

	close(sockfd);
	return 0;
}