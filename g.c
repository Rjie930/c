/*
 * @Author: vincent
 * @Date: 2023-08-29 00:16:27
 * @LastEditTime: 2023-08-29 00:43:51
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
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    // 创建一个UDP通信端点（套接字/套接口）
    int sockfd = socket(AF_INET,    /*使用IPv4做网际通信*/
                        SOCK_DGRAM, /*使用数据报方式收发数据（帧同步）*/
                        0 /*使用数据报的默认协议，即UDP*/);

    // 定义IPv4地址结构体，并存放好要绑定的地址
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.16.133");
    addr.sin_port = htons(50001); // host to network short

    // 绑定IP和端口
    bind(sockfd, (struct sockaddr *)&addr, len);

    // 静静地等待对方的数据
    struct sockaddr_in peeraddr;
    len = sizeof(peeraddr);
    char buf[100];
    while (1)
    {
        bzero(buf, 100);
        bzero(&peeraddr, len);
        recvfrom(sockfd, buf, 100, 0, (struct sockaddr *)&peeraddr, &len);

        if (strcmp(buf, "bye\n") == 0)
            break;

        printf("收到[%s:%hu]的消息:%s", inet_ntoa(peeraddr.sin_addr),
               ntohs(peeraddr.sin_port),
               buf);
    }

    return 0;
}
