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
#include <errno.h>

#define CONNECTING 0
#define CONNECT_FAIL 1
#define CONNECT_SUCCESS 2

sem_t s;

int connect_stat = CONNECTING;

void *connecting_host(void *arg)
{
    pthread_detach(pthread_self());

    fprintf(stderr, "connecting");
    while (connect_stat == CONNECTING)
    {
        fprintf(stderr, ".");
        usleep(100 * 1000);
    }
    printf("%s", connect_stat == CONNECT_SUCCESS ? "[OK]\n" : "[FAIL]\n");
    sem_post(&s);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
        perror("");

    long state = fcntl(sockfd, F_GETFL);
    state |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, state);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.2.139");
    addr.sin_port = htons(50001);

    sem_init(&s, 0, 0);
    connect_stat = CONNECTING;
    pthread_t t;
    pthread_create(&t, NULL, connecting_host, NULL);
    if (connect(sockfd, (struct sockaddr *)&addr, len) == -1 && (errno == EINPROGRESS || errno == EAGAIN))
    {
        fd_set wset;
        FD_ZERO(&wset);
        FD_SET(sockfd, &wset);

        state = fcntl(sockfd, F_GETFL);
        state &= ~O_NONBLOCK;
        fcntl(sockfd, F_SETFL, state);

        struct timeval tv={5,0};
        int n = select(sockfd+1, NULL, &wset, NULL, &tv);
        if(n == 0)
        {

            connect_stat = CONNECT_FAIL;
            sem_wait(&s);
            fprintf(stderr, "链接超时.\n");
            exit(0);
        }
        else if (FD_ISSET(sockfd, &wset))
        {
            int err;
            int len = sizeof(err);
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len);

            if(err == 0)
            {
                connect_stat = CONNECT_SUCCESS;
                sem_post(&s);
            }
            else
            {
                perror("connect error");
                exit(0);
            }
        }
    }
    else
    {
        printf("[1OK]\n");
        exit(0);
    }
    char buf[100];
    while (1)
    {
        bzero(buf, 100);
        fgets(buf, 100, stdin);

        write(sockfd, buf, strlen(buf));
    }
    close(sockfd);

    return 0;
}
