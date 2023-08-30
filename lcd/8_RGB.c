#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
int lcd;
struct fb_fix_screeninfo fixinfo;
struct fb_var_screeninfo varinfo;

int main(int argc, char const *argv[])
{
    lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("open lcd fail");
        exit(0);
    }

    if (ioctl(lcd, FBIOGET_FSCREENINFO, &fixinfo) != 0)
    {
        perror("get fix info fail");
        return -1;
    }

    if (ioctl(lcd, FBIOGET_VSCREENINFO, &varinfo) != 0)
    {
        perror("get var info fail");
        return -1;
    }
    unsigned long width = varinfo.xres;
    unsigned long height = varinfo.yres;
    unsigned long bpp = varinfo.bits_per_pixel;
    unsigned long screen_size = width * height * bpp / 8;

    int *p = mmap(NULL, 2 * screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);

    varinfo.xoffset = 0;
    varinfo.yoffset = 0;
    ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

    int colors[] = {
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0x00FF00FF,
        0x0000FFFF,
        0x00FFFF00,
        0x00FF8EFF,
        0x00DAA520,
    };
    int j = 0;

    while (1)
    {
        varinfo.xoffset = 0;
        varinfo.yoffset = ((j + 1) % 2) * varinfo.yres;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

        for (int y = 0; y < 240; y++)
            for (int x = 0; x < 800; x++)
            {
                p[y * 800 + x + screen_size / 4 * (j % 2)] = colors[(x / 200 + j) % 8];
                p[y * 800 + 240 * 800 + x + screen_size / 4 * (j % 2)] = colors[(7 - (x / 200) + j) % 8];
            }
        j++;
        sleep(1);
    }
    close(lcd);
    return 0;
}
