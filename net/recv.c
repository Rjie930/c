/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-08-29 16:37
 * @LastEditTime   : 2023-08-29 17:29
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        perror("");

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.16.133");
    addr.sin_port = htons(50001);

    if (bind(sockfd, (struct sockaddr *)&addr, len) != 0)
        perror("");

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
            
        printf("recv:%s:%hu  megs:%s", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), buf);
    }

    return 0;
}
