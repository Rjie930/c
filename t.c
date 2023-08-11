#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>

// #include <cjson/cJSON.h>
void show_png(int *p ,char *path, int x, int y);
void main(int argc, char **argv)
{
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("打开lcd失败");
        exit(0);
    }
    int *p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    show_png(p,argv[1], 200, 150);
    close(lcd);
    munmap(p, 480 * 800 * 4);
}
