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
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("打开lcd失败");
        exit(0);
    }

    int *p = mmap(NULL, 800 * 480 * 4, PROT_WRITE, MAP_SHARED, lcd, 0);
    if (p == MAP_FAILED)
    {
        perror("内存映射失败");
        exit(0);
    }
    int red = 0x00ff0000;

    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(p + i + 800 * j, &red, 4);
    }
    close(lcd);
    munmap(p, 800 * 480 * 4);
    return 0;
}
