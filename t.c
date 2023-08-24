#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    mkfifo("fifo1",0777);
    char buf[20];
// bzero(buf,20);
    int fd = open("fifo", O_RDWR);
    // if (fork() == 0)
    // {
    //     write(fd, "123321123", 10);
    //     printf("write\n");
    // }
    // sleep(2);
    // read(fd, buf, 20);
    // write(fd,"buf",10);
    char *p[] = {"ls", "." ,NULL};
    // char *pp[] = {"Name=JIE", NULL};
    strtok()
    // execvp("ls", p );
    // printf("%s\n", buf);

    return 0;
}
