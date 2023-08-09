#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    int tp = open("/dev/input/event0", O_RDWR);

    struct input_event buf;
    struct fb_fix_screeninfo fixinfo;
    struct fb_var_screeninfo varinfo;
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

    int colors[] = {
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0x00FF00FF,
        0x00000,
        0x00000,
        0x00FF8EFF,
        0x00DAA520,
    };
    int *p = mmap(NULL, 800 * 480 * 4 * 2, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);
    for (int i = 0; i < 800 * 480; i++)
    {
        p[i] = 0xffffff;
    }
    int x = 240, y = 400, i = 0;
    while (1)
    {

        read(tp, &buf, sizeof(buf));

        if (buf.type == EV_ABS)
        {
            buf.code==0?(x = buf.value * 800 / 1018):(y = buf.value * 480 / 592);
            p[y * 800 - 1 + x] = colors[i];
            p[y * 800 + x] = colors[i];

            p[y * 800 + 800 - 1 + x] = colors[i];
            p[y * 800 + 800 + x] = colors[i];

        }
        else if (buf.type == EV_KEY &&buf.code == BTN_TOUCH &&buf.value == 0)
        {
            i++;
            if (i == 8)
                i = 0;
        }
    }

    close(lcd);
    munmap(p, 800 * 400 * 4);
    return 0;
}
