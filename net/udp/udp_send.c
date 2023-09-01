#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        printf("create socket error\n");
        return -1;
    }
    // 设置广播
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    addr.sin_port = htons(55555);

    sendto(sockfd, "|in|123456", 5, 0, (struct sockaddr *)&addr, sizeof(addr));

    close(sockfd);
    return 0;
}
