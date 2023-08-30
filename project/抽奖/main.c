/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-08-29 09:33
 * @LastEditTime   : 2023-08-29 11:55
 *******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include "font.h"

pthread_t pth_show;

void sighook(int sig)
{
    printf("抽取名字\n");
}

struct lcdarg
{
    char *lcdp;
    char (*name)[20];
    int line_sum;
};

void showbitmap(bitmap *bm, int x, int y, char *p)
{
    p += x * 4 + 4 * y * 800;
    for (int j = 0; j < bm->height; j++)
    {
        for (int i = 0; i < bm->width; i++)
            memcpy(p + i * 4 + 4 * 800 * j, bm->map + i * 4 + bm->width * j * 4, 4);
    }

    bzero(bm->map, bm->width * bm->height * bm->byteperpixel);
}

void Touch_on()
{
    int tp = open("/dev/input/event0", O_RDWR);
    if (tp == -1)
    {
        perror("open input fail");
        exit(0);
    }

    struct input_event buf;

    while (1)
    {
        read(tp, &buf, sizeof(buf));
        if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
        {
            printf("按下\n");
            break;
        }
    }
}

void Touch_off()
{
    int tp = open("/dev/input/event0", O_RDWR);
    if (tp == -1)
    {
        perror("open input fail");
        exit(0);
    }

    struct input_event buf;

    while (1)
    {
        read(tp, &buf, sizeof(buf));
        if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value > 0)
        {
            printf("松开\n");
            break;
        }
    }
}

void show_png(char *path, char *p, int xyoffset);

void *getxy(void *arg)
{
    int lcd = *(int *)arg;
    struct fb_var_screeninfo varinfo;
    if (ioctl(lcd, FBIOGET_VSCREENINFO, &varinfo) != 0)
    {
        perror("get var info fail");
        return NULL;
    }
    while (1)
    {
        Touch_off();
        varinfo.yoffset = 480;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

        Touch_on();
        varinfo.yoffset = 0;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);
        pthread_kill(pth_show, SIGCHLD);
    }
}

void *show(void *arg)
{
    struct lcdarg larg = *(struct lcdarg *)arg;
    font *f5 = fontLoad("simfang.ttf");
    fontSetSize(f5, 65);
    bitmap *Linit;
    Linit = createBitmapWithInit(800, 100, 4, 0xff);
    while (1)
    {
        pause();
        for (size_t i = 1, j; i < 200; i += i)
        {
            fontPrint(f5, Linit, 0, 0, (char*)(larg.name + rand() % larg.line_sum), 0xffff0000, 0);
            showbitmap(Linit, 290, 60, larg.lcdp);
            usleep(10 * 1000 * i);
        }
    }
}



int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("");
        exit(0);
    }
    struct fb_var_screeninfo varinfo;
    if (ioctl(lcd, FBIOGET_VSCREENINFO, &varinfo) != 0)
    {
        perror("get var info fail");
        return -1;
    }

    char *lcdp = mmap(NULL, 800 * 480 * 4 * 2, PROT_WRITE, MAP_SHARED, lcd, 0);
    if (lcdp == NULL)
    {
        perror("");
        exit(0);
    }

    int line_sum = 0;
    FILE *fp = fopen("file", "r");
    if (fp==NULL)
    {
       perror("");
    }
    
    while (!feof(fp))
    {
        if (fgetc(fp) == 10)
            line_sum++;
    }
    char(*p)[20] = calloc(line_sum, line_sum * 20);
    fseek(fp, 0, SEEK_SET);
    printf("%d\n", __LINE__);
    for (int i = 0; i < line_sum; i++)
    {
        fgets((char*)(p + i), 20, fp);
        *strchr((char *)(p + i), '\n') = '\0';
    }

    int bg = 0x00;
    for (int j = 0; j < 480 * 2; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(lcdp + i * 4 + 4 * 800 * j, &bg, 4);
    }
    show_png("on.png", lcdp, 0);
    show_png("off.png", lcdp, 1);

    struct lcdarg arg;
    arg.lcdp = lcdp;
    arg.name = p;
    arg.line_sum = line_sum;

    signal(SIGCHLD, sighook);
    pthread_t pth_getxy;
    pthread_create(&pth_getxy, NULL, getxy, (void *)&lcd);
    pthread_create(&pth_show, NULL, show, (void *)&arg);

    // pthread_exit(NULL);
    pthread_join(pth_show,NULL);
}
