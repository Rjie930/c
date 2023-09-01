#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct timeval val = {.tv_sec = 3,
                          .tv_usec = 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(55555);

    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    // 接收数据
    int ret;
    char buf[10] = {0};
    struct sockaddr_in from;
    socklen_t len = sizeof(from);
    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &len);
    if (ret == -1)
    {
        perror("recvfrom");
        exit(0);
    }
    
    printf("receive from %s:%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
    printf("%s\n", buf);
    
    close(sockfd);
    return 0;
}
