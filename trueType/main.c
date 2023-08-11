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

pthread_cond_t cond_get_info = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_info = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_init = PTHREAD_MUTEX_INITIALIZER;

int black = 0x0;
int bgcolor = 0xffffffff;
int red = 0xff000000;
int yellow = 0xffff0000;

char buf_get[600];
char buf_mesg[300];
char buf_wea[200];
char buf_tips[100];
char time_data[80];
char buf_img[10];
char path[15];

void show_png(int *p, char *path, int x, int y);

void showbitmap(bitmap *bm, int x, int y, int *p)
{
    p += x + y * 800;
    for (int j = 0; j < bm->height; j++)
    {
        for (int i = 0; i < bm->width; i++)
            memcpy(p + i + 800 * j, bm->map + i * 4 + bm->width * j * 4, 4);
    }

    bzero(bm->map, bm->width * bm->height * bm->byteperpixel);
}

void *getInfo(void *arg)
{
    pthread_mutex_lock(&mutex_info);
    int *p = (int *)arg;
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

    // wea
    font *f3 = fontLoad("simfang.ttf");
    fontSetSize(f3, 50);
    bitmap *Lwea;
    Lwea = createBitmapWithInit(500, 210, 4, black);

    while (1)
    {
        bzero(buf_get, 600);

        int n = recvfrom(sockfd, buf_get, 600, 0, NULL, NULL);
        printf("recvfrom \n");
        pthread_cond_signal(&cond_get_info);
        printf("cond_get_info \n");

        pthread_mutex_lock(&mutex_init);
               printf("mutex_init \n");
        bzero(buf_wea, 200);
        bzero(buf_mesg, 300);
        printf("%s\n", buf_get);
        sscanf(buf_get, "%s %ld %s %s %[^\r]", buf_img, &getTime, buf_mesg, buf_tips, buf_wea);
        getTime += 28800;
        timeInfo = localtime(&getTime);
        strftime(format, sizeof(format), "%Y-%m-%d %H:%M:%S", timeInfo);

        char command[100];
        snprintf(command, sizeof(command), "date -s \"%s\"", format);
        system(command);
        printf("收到数据：\nimg:%s\ngetTime:%ld\nbuf_mesg:%s\nbuf_wea:%s\ncommand:%s\ntips:%s\n", buf_img, getTime, buf_mesg, buf_wea, command, buf_tips);
        sprintf(path, "img/%s.png", buf_img);

        fontPrint(f3, Lwea, 4, 4, buf_wea, red, 0);
        showbitmap(Lwea, 0, 80, p);

        // img
        show_png(p, path, 550, 120);

        pthread_mutex_unlock(&mutex_info);
        
    }
    pthread_exit(NULL); // 结束线程
}

void *getTime(void *arg) // 固定格式
{
    int *p = (int *)arg;
    // 时间框
    font *f = fontLoad("simfang.ttf");
    fontSetSize(f, 55);
    bitmap *Ltime;
    Ltime = createBitmapWithInit(800, 70, 4, black);
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

void *init(void *arg) // 固定格式
{
    pthread_mutex_lock(&mutex_init);
    int *p = (int *)arg;

    // init
    font *f5 = fontLoad("simfang.ttf");
    fontSetSize(f5, 65);
    bitmap *Linit;
    Linit = createBitmapWithInit(800, 350, 4, black);
    fontPrint(f5, Linit, 0, 0, "富强、民主、文明、和谐\n\n自由、平等、公正、法治\n\n爱国、敬业、诚信、友善", red, 0);
    showbitmap(Linit, 60, 60, p);

    pthread_cond_wait(&cond_get_info, &mutex_init);
    // 清屏
    for (int j = 0; j < 400; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(p + i + 800 * j, &black, 4);
    }
    pthread_mutex_unlock(&mutex_init);
    printf("init exit\n");
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

    int *p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if (p == NULL)
    {
        printf("内存映射失败\n");
        exit(0);
    }
    pthread_t thread_getInfo;
    pthread_t thread_getTime;
    pthread_t thread_init;

    pthread_create(&thread_getInfo, NULL, getInfo, (void *)p);
    
    pthread_create(&thread_init, NULL, init, (void *)p);

    // 清屏
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
            memcpy(p + i + 800 * j, &black, 4);
    }

    // 单位
    font *f1 = fontLoad("simfang.ttf");
    fontSetSize(f1, 55);
    bitmap *Lwho;
    Lwho = createBitmapWithInit(800, 60, 4, black);
    fontPrint(f1, Lwho, 530, 0, "--粤嵌科技", yellow, 0);
    showbitmap(Lwho, 0, 420, p);

    pthread_mutex_lock(&mutex_info);
    pthread_create(&thread_getTime, NULL, getTime, (void *)p);

    // mesg
    font *f2 = fontLoad("simfang.ttf");
    fontSetSize(f2, 55);
    bitmap *Lmesg;
    Lmesg = createBitmapWithInit(800, 60, 4, black);

    // tips
    font *f4 = fontLoad("simfang.ttf");
    fontSetSize(f4, 40);
    bitmap *Ltips;
    Ltips = createBitmapWithInit(800, 50, 4, black);

    int i = 0;
    int j = 0;
    while (1)
    {
        // mesg
        fontPrint(f2, Lmesg, 0 + i, 0, buf_mesg, red, 0);
        showbitmap(Lmesg, 800, 355, p);

        // tips
        fontPrint(f4, Ltips, 0 + j, 0, buf_tips, red, 0);
        showbitmap(Ltips, 800, 300, p);

        i -= 3;
        if (-i > strlen(buf_mesg) * 16)
        {
            i = 0;
        }
        j -= 3;
        if (-j > strlen(buf_tips) * 14)
        {
            j = 0;
        }
    }
    pthread_join(thread_getInfo, NULL);
    close(lcd);
    munmap(p, 480 * 800 * 4);
    return 0;
}
