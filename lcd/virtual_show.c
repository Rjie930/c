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

    printf("x:%d,y:%d\n", varinfo.xres, varinfo.yres);
    printf("v_x:%d,v_y:%d\n", varinfo.xres_virtual, varinfo.yres_virtual);
    printf("bits: %d\n", varinfo.bits_per_pixel);

    unsigned long width = varinfo.xres;
    unsigned long height = varinfo.yres;
    unsigned long bpp = varinfo.bits_per_pixel;
    unsigned long screen_size = width * height * bpp / 8;

    int *p = mmap(NULL, 3 * screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);

    varinfo.xoffset = 0;
    varinfo.yoffset = 0;
    ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

    int buf[4] = {0xff, 0xff00, 0xff0000};
    int j = 0;

    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 800 * 480; i++)
            p[i + screen_size/4 * k] = buf[k];

    while (1)
    {
        varinfo.xoffset = 0;
        varinfo.yoffset = j * varinfo.yres;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

        if (++j == 3)
            j = 0;
    }
    close(lcd);
    return 0;
}
