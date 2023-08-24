#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    int fd[2];
    pipe(fd);
    int ret = fork();
    if (ret == 0)
    {
        close(fd[0]);
        // printf("son:%d\n", getpid());
        // execl("./t","./t","3",NULL);
        sleep(2);
        // exit(123);
        // pause();
        while (1)
        {
            // sleep(1);
        }
        write(fd[1], "1234", 5);
    }
    else
    {
        printf("儿砸:%d\n", ret);
        close(fd[1]);
        char buf[5];
        int stat;
        // read(fd[0], buf, 5);
        // printf("pipe:%s\n", buf);
        waitpid(-1, &stat, WCONTINUED);
        if (WIFSTOPPED(stat))
        {
            printf("Child process was stopped by signal: %d\n", WSTOPSIG(stat));
        }
        else if (WIFEXITED(stat))
        {
            printf("Child process exited with stat: %d\n", WEXITSTATUS(stat));
        }
    }
}

// int main()
// {
//     // 创建具名管道
//     // mkfifo("/tmp/fifo", 0666);

//     // 向管道写入数据
//     int fd = open("fifo", O_RDWR);

//     if (fork()==0)
//     {
//         execl("./t","./t",NULL);
//     }
//     // sleep(1);
//     char *msg = "data from FIFO";
//     write(fd, msg, strlen(msg));
//     // sleep(5);
//     close(fd);
//     return 0;
// }