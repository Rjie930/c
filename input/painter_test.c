#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/input.h> // 系统定义输入设备操作的API
#include <linux/fb.h> // 系统定义输入设备操作的API
#include <sys/mman.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

struct xy
{
    int x;
    int y;
};

char *fbm; // 显存入口指针
struct xy pos;
sem_t s;
int begin = 1;

int lcdw, lcdh, bpp;

// 以pos为中心，画一个九宫格（线的粗细为：3像素）
void setPixel(struct xy pos, int color)
{
    char *tmp = fbm - lcdw*bpp/8 - 4;
    for(int j=0; j<3; j++)
    {
        tmp += (lcdw*bpp/8) * j;

        for(int i=0; i<3; i++)
        {
            memcpy(tmp + (pos.y*lcdw+pos.x)*bpp/8 + 4*i, &color, 4);
        }
    }
}

void drawline(struct xy pos1, struct xy pos2)
{
    int di = 0;

    int gx;
    int gy;
    gx = (pos2.x > pos1.x) ? 1 : -1;
    gy = (pos2.y > pos1.y) ? 1 : -1;

    int dx;
    int dy;
    dx = (pos2.x - pos1.x) * gx + 1;
    dy = (pos2.y - pos1.y) * gy + 1;

    int dc = 2;

    int color = 0x00000000;
    if(dx >= dy)
    {
        for(/*x2 += gx*/; pos1.x != pos2.x; pos1.x += gx)
        {
            di += dy;
            if(di <= dx)
            {
                setPixel(pos1, color);
                continue;
            }
            while((di > dx) && (pos1.y != pos2.y))
            {
                di -= dx;
                pos1.y += gy;
                setPixel(pos1, color);
            }
        }
    }
    else
    {
        for(/*y2 += gw*/; pos1.y != pos2.y; pos1.y += gy)
        {
            di += dx;
            if(di <= dy)
            {
                setPixel(pos1, color);
                continue;
            }
            while((di > dy) && (pos1.x != pos2.x))
            {
                di -= dy;
                pos1.x += gx;
                setPixel(pos1, color);
            }
        }
    }
}

void *drawLines(void *arg)
{
    struct xy pos1 = {0};
    struct xy pos2 = {0};

    while(1)
    {
        sem_wait(&s);
        pos1.x = pos.x;
        pos1.y = pos.y;

        while(1)
        {
            sem_wait(&s);

            if(begin == 1)
            {
                begin = 0;
                break;
            }

            // draw line
            pos2 = pos;

            drawline(pos1, pos2);

            // store postions
            pos1 = pos2;
        }
    }
}

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);

    struct fb_var_screeninfo vinfo;
    ioctl(lcd, FBIOGET_VSCREENINFO, &vinfo);

    lcdw = vinfo.xres;
    lcdh = vinfo.yres;
    bpp  = vinfo.bits_per_pixel;
    fbm = mmap(NULL, lcdw*lcdh*bpp/8*2, PROT_WRITE, MAP_SHARED, lcd, 0);
    if(fbm == MAP_FAILED)
    {
        perror("映射内存失败");
        exit(0);
    }
    // bzero(fbm, lcdw*lcdh*bpp/8);
    memset(fbm,0xff,lcdw*lcdh*bpp/8);

    sem_init(&s, 0, 0);

    // draw lines
    pthread_t tid;
    pthread_create(&tid, NULL, drawLines, NULL);

    // get x and y
    int tp = open("/dev/input/event0", O_RDWR);
    struct input_event buf;
    while(1)
    {
        bzero(&buf, sizeof(buf));
        read(tp, &buf, sizeof(buf));

        if (buf.type == EV_ABS && buf.code == ABS_X)
        {
            pos.x = buf.value*800/1018;
            printf("x  %d\n",pos.x);
        }
        if (buf.type == EV_ABS && buf.code == ABS_Y)
        {
            pos.y = buf.value*480/585;
            printf("y  %d\n",pos.y);
        }

        if (buf.type == EV_SYN)
        {
            sem_post(&s);
        }

        if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
        {
            begin = 1;
        }
    }

    return 0;
}