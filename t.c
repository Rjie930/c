#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include "data_struct/kernel_list/kernel_list.h"
#include "font.h"

int done;
typedef struct node
{
    char img_path[50];
    struct list_head list;
} listnode, *linklist;

linklist init_list()
{
    linklist head = calloc(1, sizeof(listnode));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

linklist new_node(char *img_path)
{
    linklist new = calloc(1, sizeof(listnode));
    if (new != NULL)
    {
        strcpy(new->img_path, img_path);
        INIT_LIST_HEAD(&new->list);
    }
    return new;
}

void show(linklist head)
{
    struct node *pos;
    list_for_each_entry(pos, &head->list, list)
    {
        printf("%s\n", pos->img_path);
    }
}

int get_xy_s(int *x, int *y)
{
    int ty, tx;
    int fd = open("/dev/input/event0", O_RDWR);
    if (fd < 0)
    {
        perror("open touch fail:");
        return -1;
    }
    while (1)
    {
        struct input_event xy;
        while (1)
        {
            read(fd, &xy, sizeof(xy));
            if (xy.type == EV_ABS)
            {
                xy.code == REL_X ? (*x = xy.value * 800 / 1020) : (*y = xy.value * 480 / 592);
                // printf("(%d,%d)\n", *x, *y);
            }
            if (xy.type == EV_KEY && xy.code == BTN_TOUCH)
            {
                // printf("touch %d\n",xy.value);
                if (xy.value == 1)
                {
                    // printf("press\n");
                    tx = *x;
                    ty = *y;
                }
                if (xy.value == 0)
                {
                    // printf("up\n");
                    // 左
                    if ((*x - tx < -50))
                        return 1;
                    // 右
                    if ((*x - tx > 50))
                        return 2;
                    // 上
                    if (*y - ty < -50)
                        return 3;
                    // 下
                    if (*y - ty > 50)
                        return 4;
                    // printf("%s%s\n", (x - tx == 0) ? "" : (x - tx < 0) ? "左": "右",(y - ty == 0) ? "" : (y - ty < 0) ? "上" : "下");
                    return 0;
                }
            }
        }
    }
    close(fd);
}

void show_jpg(char const *image_path, char *p, int xyoffse);
void show_bmp(char const *image_path, char *p, int xyoffse);
void show_png(char const *image_path, char *p, int xyoffse);

void load_image(linklist pos, char *p, int xyoffset)
{
    memset(p + xyoffset * 800 * 480 * 4, 0, 800 * 480 * 4);
    if (strstr(pos->img_path, ".jpg"))
        show_jpg(pos->img_path, p, xyoffset);
    else if (strstr(pos->img_path, ".bmp"))
        show_bmp(pos->img_path, p, xyoffset);
    else if (strstr(pos->img_path, ".png"))
        show_png(pos->img_path, p, xyoffset);
    printf("load:%s\n", pos->img_path);
}
linklist head;

void *getImg(void *arg)
{
    char *src=(char*)arg;
    DIR *dp = opendir(src);
    // printf("cd %s\n", src);
    chdir(src);

    struct dirent *ep;
    struct stat entry_desfo;
    char entry_src[100];
    char entry_des[100];
    bzero(entry_src, 100);

    ep = readdir(dp);
    while (ep != NULL)
    {
        while (ep != NULL && (!strcmp(ep->d_name, ".") | !strcmp(ep->d_name, "..")))
            ep = readdir(dp);

        if (ep == NULL)
        {
            break;
        }

        stat(ep->d_name, &entry_desfo);
        getcwd(entry_src, 100);

        sprintf(entry_src, "%s/%s", src, ep->d_name);

        if (S_ISDIR(entry_desfo.st_mode))
        {
            // printf("entry dir: %s\n", ep->d_name);
            getImg((void*)entry_src);
            chdir("..");
        }
        else
        {
            if (strstr(entry_src, ".png") || strstr(entry_src, ".jpg") || strstr(entry_src, ".bmp"))
            {
                // printf("file: %s\n", entry_src);
                linklist new = new_node(entry_src);
                list_add_tail(&new->list, &head->list);
                printf("%s\n", entry_src);
            }
        }
        ep = readdir(dp);
    }
    // printf("cd %s\n", "..");
    closedir(dp);
}

void showbitmap(bitmap *bm, int x, int y, char *p)
{
    p += x*4 + y * 800*4;
    for (int j = 0; j < bm->height; j++)
    {
        for (int i = 0; i < bm->width; i++)
            memcpy(p + i*4 + 800 * j*4, bm->map + i * 4 + bm->width * j * 4, 4);
    }
    // bzero(bm->map, bm->width * bm->height * bm->byteperpixel);
}

int main(int argc,char **argv)
{
    head = init_list();
    pthread_t thread_getImg;
    pthread_create(&thread_getImg,NULL,getImg,(void*)argv[1]);

    
    int lcd = open("/dev/fb0", O_RDWR);
    char *p = mmap(NULL, 800 * 480 * 4 * 3, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);

    // 获取lcd屏幕信息
    struct fb_var_screeninfo varinfo;
    if (ioctl(lcd, FBIOGET_VSCREENINFO, &varinfo) != 0)
    {
        perror("get var info fail");
        return -1;
    }


    font *f1 = fontLoad("simfang.ttf");
    fontSetSize(f1, 55);
    bitmap *Lwho;
    Lwho = createBitmapWithInit(800, 60, 4, black);
    fontPrint(f1, Lwho, 530, 0, "--粤嵌科技", yellow, 0);
    showbitmap(Lwho, 0, 420, p);
    // 递归遍历指定目录
    // getImg("/root/zz/c", head);
    while (1)
    {
        if(pthread_tryjoin_np(thread_getImg, NULL)==0);
        {
            break;
        }
        printf("wait~\n");
        usleep(100000);
    }
    
    // 分别加载三张图片到显存
    linklist pos = list_entry(&(head->list), typeof(*pos), list);
    for (int i = 0; i < 3; i++)
    {
        pos = list_entry(pos->list.next, typeof(*pos), list);
        load_image(pos, p, i);
    }
    pos = list_entry(pos->list.prev, typeof(*pos), list);

    int bg = 0x0;
    int i = 0, x, y, s;
    int block = 0;
    int now = 0, prev = 1, next = 2, tmp_now, tmp_prev;
    varinfo.xoffset = 0;
    while (1)
    {
        s = get_xy_s(&x, &y);
        if (s == 1)
        {
            // 左、next
            pos = list_entry(pos->list.next->next, typeof(*pos), list);
            if (pos == head)
                pos = list_entry(pos->list.next, typeof(*pos), list);
            printf("%s\n", pos->img_path);

            //更新三个显存的位置
            tmp_now = now;
            now = next;
            next = prev;
            prev = tmp_now;
            varinfo.yoffset = now * varinfo.yres;
            ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);
            load_image(pos, p, next);
            pos = list_entry(pos->list.prev, typeof(*pos), list);
        }
        else if (s == 2)
        {
            // 右、prev
            pos = list_entry(pos->list.prev->prev, typeof(*pos), list);
            if (pos == head)
                pos = list_entry(pos->list.prev, typeof(*pos), list);
            printf("%s\n", pos->img_path);
            tmp_now = now;
            now = prev;
            prev = next;
            next = tmp_now;
            varinfo.yoffset = now * varinfo.yres;
            ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);
            load_image(pos, p, prev);
            pos = list_entry(pos->list.next, typeof(*pos), list);
        }
        else if (s == 3)//退出程序
        {
            memset(p + 800 * 480 * 4 * now, 0, 800 * 480 * 4);
            break;
        }
    }
    close(lcd);
    munmap(p, 800 * 400 * 4 * 3);
}
