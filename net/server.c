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
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
    {
        perror("");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(50001);

    if (bind(sockfd, (struct sockaddr *)&addr, len) != 0)
    {
        perror("");
        return -1;
    }

    listen(sockfd, 3);

    struct sockaddr peeraddr;
    len = sizeof(peeraddr);
    bzero(&peeraddr, len);

    int connfd = accept(sockfd, &peeraddr, &len);

    int ret;
    char buf[100];
    while (1)
    {
        bzero(buf, 100);

        ret = read(connfd, buf, 100);
        printf("ret:%d\n", ret);

        if (strcmp(buf, "bye\n") == 0)
            break;

        printf("recv:%s  megs:%s", inet_ntoa(((struct sockaddr_in *)&peeraddr)->sin_addr), buf);
        sleep(1);
    }

    return 0;
}
