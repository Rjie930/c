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
#include <netdb.h>

int main(int argc, char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, len);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("110.64.203.71");

    if (connect(sockfd, (struct sockaddr *)&addr, len))
        printf("connect ok\n");

    char *http_request = "GET /attachment/sites/item/2022_06/02_11/6da61a7dd2114274.png HTTP/1.1\r\n"
                         "Host: attach.seig.edu.cn\r\n\r\n";

    write(sockfd, http_request, strlen(http_request));

    char response_header[4096];
    int header_len = 0;
    int total_len = 0;
    char c;
    while (read(sockfd, &c, 1) > 0)
    {
        response_header[header_len++] = c;
        total_len++;
        if (header_len >= 4 && response_header[header_len - 4] == '\r' && response_header[header_len - 3] == '\n' &&
            response_header[header_len - 2] == '\r' && response_header[header_len - 1] == '\n')
        {
            break;
        }
    }
    response_header[header_len] = '\0';
    int len1 = atoi(strstr(response_header, "Content-Length") + 16);
    printf("%d\n", len1);
    int ret;
    char str[10];
    int fd = open("1.png", O_CREAT | O_RDWR | O_TRUNC, 0666);
    while (1)
    {
        ret = read(sockfd, str, 10);
        write(fd, str, ret);
        if (ret < 10)
            break;
    }

    close(fd);
    close(sockfd);
    return 0;
}