#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "font.h"
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int bgcolor  =0x0;
int red = 0xff000000;
int yellow= 0xffff0000;

char buf_get[600];
char buf_mesg[300];
char buf_wea[200];
char buf_tips[100];
char time_data[80];

void showbitmap(bitmap *bm, int x, int y, char *p)
{
    // 准备好LCD设备（如果多次显示，应该只做一遍）
    for (int j = 0; j < bm->height; j++)
    {
        for (int i = 0; i < bm->width; i++)
            memcpy(p + i * 4 + 800 * j + x * 4 + y * 800 * 4, bm->map + i * 4 + 800 * j, 4);
    }
    // printf("-------------------\n");
    // printf("width:%d\n", bm->width);
    // printf("height:%d\n", bm->height);
    // printf("byteperpixel:%d\n", bm->byteperpixel);
    // printf("-------------------\n");
    bzero(bm->map, bm->width * bm->height * bm->byteperpixel);
    // 字体的信息：
    // bm->width;
    // bm->height;
    // bm->byteperpixel;
    // bm->map; // 指向RGB数据，没有上下颠倒，也没有一行4字节倍数的约束，就是纯粹的RGB
}

void *getInfo(void *arg)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.16.134");
    addr.sin_port = htons(50001);
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    time_t getTime;
    struct tm *timeInfo;
    char format[80];

    while (1)
    {
        bzero(buf_get, 600);

        int n = recvfrom(sockfd, buf_get, 600, 0, NULL, NULL);
        bzero(buf_wea, 200);
        bzero(buf_mesg, 300);
        printf("%s\n",buf_get);
        sscanf(buf_get, "%ld %s %s %[^\r]", &getTime, buf_mesg,buf_tips ,buf_wea);
        getTime += 28800;
        timeInfo = localtime(&getTime);
        strftime(format, sizeof(format), "%Y-%m-%d %H:%M:%S", timeInfo);

        char command[100];
        snprintf(command, sizeof(command), "date -s \"%s\"", format);
        system(command);
        printf("收到数据：\ngetTime:%ld\nbuf_mesg:%s\nbuf_wea:%s\ncommand:%s\ntips:%s\n", getTime, buf_mesg, buf_wea, command,buf_tips);
    }
    pthread_exit(NULL); // 结束线程
}

void *getTime(void *arg) // 固定格式
{
    char *p=(char*)arg;
    // 时间框
    font *f = fontLoad("simfang.ttf");
    fontSetSize(f, 55);
    bitmap *Ltime;
    Ltime = createBitmapWithInit(800, 200, 4, bgcolor);
    time_t rawTime;
    struct tm *timeInfo;
    while (1) // 用于等待接收主机的传送信号
    {
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        strftime(time_data, sizeof(time_data), "%Y-%m-%d %H:%M:%S %A", timeInfo);
        fontPrint(f, Ltime, 10, 0, time_data, yellow, 0);
        showbitmap(Ltime, 0, 0, p);
        sleep(1);
    }
    pthread_exit(NULL); // 结束线程
}

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("打开lcd失败");
        exit(0);
    }

    char *p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if (p == NULL)
    {
        printf("内存映射失败\n");
        exit(0);
    }
    pthread_t thread_getInfo;
    pthread_t thread_getTime;

    pthread_create(&thread_getInfo, NULL, getInfo, NULL);
    pthread_create(&thread_getTime, NULL, getTime, (void*)p);

    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(p + i * 4 + 800 * j * 4, &bgcolor, 4);
    }

    // 单位
    font *f1 = fontLoad("simfang.ttf");
    fontSetSize(f1, 55);
    bitmap *Lwho;
    Lwho = createBitmapWithInit(800, 200, 4, bgcolor);
    fontPrint(f1, Lwho, 530, 0, "--粤嵌科技", 0xffff0000, 0);
    showbitmap(Lwho, 800, 400, p);

    // wea
    font *f3 = fontLoad("simfang.ttf");
    fontSetSize(f3, 40);
    bitmap *Lwea;
    Lwea = createBitmapWithInit(800, 490, 4, bgcolor);

    // mesg
    font *f2 = fontLoad("simfang.ttf");
    fontSetSize(f2, 55);
    bitmap *Lmesg;
    Lmesg = createBitmapWithInit(800, 200, 4, bgcolor);

    // tips
    font *f4 = fontLoad("simfang.ttf");
    fontSetSize(f4, 40);
    bitmap *Ltips;
    Ltips = createBitmapWithInit(800, 170, 4, bgcolor);
    int i = 0;
    int j=0;
    while (1)
    {
        // mesg
        fontPrint(f2, Lmesg, 0 + i, 0, buf_mesg, red, 0);
        showbitmap(Lmesg, 800, 330, p);

        // wea
        fontPrint(f3, Lwea, 0, 0, buf_wea, red, 0);
        showbitmap(Lwea, 800, 120, p);

        // tips
        fontPrint(f4, Ltips, 0+j, 0, buf_tips, red, 0);
        showbitmap(Ltips, 800, 255, p);
        i -= 10;
        if (-i>strlen(buf_mesg)*16)
        {
            i=0;
        }
        j -= 10;
        if (-j>strlen(buf_tips)*14)
        {
            j=0;
        }
        // usleep(1000000);
        // printf("%d\n",i);
    }
    pthread_join(thread_getInfo, NULL);
    close(lcd);
    munmap(p, 480 * 800 * 4);
    return 0;
}
