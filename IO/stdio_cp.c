#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sysmacros.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("打开文件%s失败%s",argv[1],strerror(errno));
        exit(0);
    }
    int fd1 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd1 == -1)
    {
        printf("打开文件%s失败%s",argv[2],strerror(errno));
        exit(0);
    }
    char buf[512];
    int ret=1;
    while (ret>0)
    {
        bzero(buf, 512);
         ret= read(fd, buf, 5);
        if (ret <= 0)
        {
            if (ret=-1)
            {
                perror("write");
            }
            printf("done!\n");
            break;
        }
        write(fd1, buf, ret);
        printf("write:%d\n", ret);
    }
    close(fd);
    close(fd1);
}