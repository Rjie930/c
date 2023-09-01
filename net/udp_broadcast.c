/*
 * @Author: vincent
 * @Date: 2023-08-30 00:25:38
 * @LastEditTime: 2023-08-30 01:00:29
 */
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
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sockfd;
#define BUFSIZE 200

// 可以给本局域网内的其他主机发送广播消息
// 可以接收本局域网内其他主机的消息

void *broadcast(void *arg)
{
    pthread_detach(pthread_self());

    // 1. 使能sockfd的广播属性
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    bzero(&peeraddr, len);

    peeraddr.sin_family = AF_INET;
    peeraddr.sin_addr.s_addr = inet_addr("192.168.16.255") ; // 2. 收件人地址写广播地址
    peeraddr.sin_port = htons(50001);

    char msg[BUFSIZE];
    while(1)
    {
        bzero(msg, BUFSIZE);
        fgets(msg, BUFSIZE, stdin);

        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&peeraddr, len);
    }
}

void *recvdata(void *arg)
{
    pthread_detach(pthread_self());

    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    char buf[BUFSIZE];
    while(1)
    {
        bzero(buf, BUFSIZE);
        bzero(&peeraddr, len);
        recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&peeraddr, &len);

        printf("收到[%s:%hu]的消息:%s", inet_ntoa(peeraddr.sin_addr),
                                        ntohs(peeraddr.sin_port),
                                        buf);
    }
}

int main(int argc, char const *argv[])
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 指定网口名称 ens33
    // 获取指定网口IP地址
    struct ifreq ifr;
    strcpy(ifr.ifr_ifrn.ifrn_name, "ens37");
    ioctl(sockfd, SIOCGIFADDR, &ifr);

    // 准备好地址结构体
    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    bzero(&myaddr, len);
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr   = ((struct sockaddr_in *)(&ifr.ifr_ifru.ifru_addr))->sin_addr; // 自动获取IP地址
    myaddr.sin_port = htons(50001);

    // 绑定sockfd和地址
    if(bind(sockfd, (struct sockaddr *)&myaddr, len) != 0)
    {
        perror("绑定地址失败");
        exit(0);
    }

    // 线程1：发送UDP广播消息
    // 线程2：接收UDP消息
    pthread_t t1, t2;
    pthread_create(&t1, NULL, broadcast, NULL);
    pthread_create(&t2, NULL, recvdata, NULL);

    pthread_exit(NULL);
}
