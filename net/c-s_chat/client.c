/*
 * @Author: vincent
 * @Date: 2023-08-31 11:05:43
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-31 00:31:26
 * @Description: 
 */
#include "head.h"

struct sockaddr_in logaddr, msgaddr;
int ID;
int sockfd;

void Login(int sockfd)
{
    srand(time(NULL));
    char loginMsg[20];
    ID = rand()%100000; // 12345

    snprintf(loginMsg, 20, "|in|%d|", ID);
    sendto(sockfd, loginMsg, strlen(loginMsg), 0,
           (struct sockaddr *)&logaddr, sizeof(logaddr));
}

// 信号响应函数
void Logout(int sig)
{
    char logoutMsg[20];
    snprintf(logoutMsg, 20, "|out|%d|", ID);
    sendto(sockfd, logoutMsg, strlen(logoutMsg), 0,
           (struct sockaddr *)&logaddr, sizeof(logaddr));
    exit(0);
}


void *recvMsg(void *arg)
{
    char buf[100];
    while(1)
    {
        bzero(buf, 100);
        recvfrom(sockfd, buf, 100, 0, NULL, NULL);

        printf("收到：%s", buf);
    }
}

void *sendMsg(void *arg)
{
    char buf[100];
    while(1)
    {
        bzero(buf, 100);
        snprintf(buf, 100, "%d|", ID); // buf: "12345|...."
        fgets(buf+6, 100, stdin);      // buf: "12345|hello~..."

        sendto(sockfd, buf, strlen(buf), 0,
              (struct sockaddr *)&msgaddr, sizeof(msgaddr));
    }
}
int main(int argc, char **argv) // ./client 服务器的IP
{
    if(argc != 2)
    {
        printf("请指定服务器IP\n");
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    socklen_t len = sizeof(struct sockaddr_in);
    bzero(&logaddr, len);
    bzero(&msgaddr, len);

    logaddr.sin_family = AF_INET;
    logaddr.sin_addr.s_addr = inet_addr(argv[1]);
    logaddr.sin_port = LOGPORT;

    msgaddr.sin_family = AF_INET;
    msgaddr.sin_addr.s_addr = inet_addr(argv[1]);
    msgaddr.sin_port = MSGPORT;

    // 登入
    Login(sockfd);

    // 登出
    signal(SIGINT, Logout);

    pthread_t t;
    pthread_create(&t, NULL, recvMsg, NULL);
    pthread_create(&t, NULL, sendMsg, NULL);

    pthread_exit(NULL);
}