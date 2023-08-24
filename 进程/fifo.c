#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    // 创建具名管道
    // mkfifo("/tmp/fifo", 0666);

    // 向管道写入数据
    int fd = open("fifo", O_RDWR);

    if (fork()==0)
    {
        execl("./t","./t",NULL);
    }
    // sleep(1);
    char *msg = "data from FIFO";
    write(fd, msg, strlen(msg));
    // sleep(5);
    close(fd);
    return 0;
}