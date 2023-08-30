#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

    int buf[3] = {0x000000ff, 0x0000ff00, 0x00ff0000};
    int j = 0;
    while (1)
    {
        lcd = open("/dev/fb0", O_RDWR);
        putchar(j + 48);
        putchar(10);
        for (int i = 0; i < 800 * 480; i++)
            write(lcd, &buf[j], 4);
        j++;
        if (j == 3)
            j = 0;
        sleep(1);
        close(lcd);
    }

    return 0;
}
