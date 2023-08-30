#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getBt()
{
    int fd = open("/dev/gecBt", O_RDONLY);
    int stat, btn=0, repeat = 0;
    while (read(fd, &stat, sizeof(stat)))
    {
        // if (repeat ^= 1)
        //     continue;
        printf("stat:%#x\n" ,stat);
        if (stat & 1)
        {
            btn = 1;
        }
        else if (stat & 1 << 8)
        {
            btn = 2;
        }
        else if (stat & 1 << 16)
        {
            btn = 3;
        }
        else if (stat & 1 << 24)
        {
            btn = 4;
        }
        else
        {
            continue;
        }
        printf("btn:%d\n" ,btn);
    }
    close(fd);
        printf("btn:%d\n" ,btn);
        return btn;
}

void changeColor(int *p, int Color)
{
   int c=Color;
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(p + i + 800 * j, &c, 3);
    }
}

void main(int argc, char const *argv[])
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

    int colors[] = {
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0x0
    };

    int fd = open("/dev/gecBt", O_RDONLY);
    int stat, btn=0, repeat = 0;
    while (read(fd, &stat, sizeof(stat)))
    {
        // if (repeat ^= 1)
        //     continue;
        if (stat & 1)
        {
            btn = 1;
        }
        else if (stat & 1 << 8)
        {
            btn = 2;
        }
        else if (stat & 1 << 16)
        {
            btn = 3;
        }
        else if (stat & 1 << 24)
        {
            btn = 4;
        }
        else
        {
            continue;
        }
        printf("btn:%d\n" ,btn);
        changeColor(p, colors[btn-1]);
    }

    close(lcd);
    munmap(p, 800 * 480 * 4);
}
