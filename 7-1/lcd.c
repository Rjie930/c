#include "head.h"

struct fb_var_screeninfo finfo;

/*
 * @Author: vincent
 * @Date: 2023-08-05 09:13:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-07 00:09:06
 * @Description: 
 */

#include "./include/head.h"

struct fb_var_screeninfo finfo;

void init_lcd(struct lcd_info *p)
{
    if(p == NULL)
        return;

    static bool initialized = false;

    // 若已初始化，那么直接返回
    if(initialized)
        return;

    p->lcd = open("/dev/fb0", O_RDWR);
    if(p->lcd == -1)
    {
        perror("打开液晶屏失败");
        return;
    }

    if(ioctl(p->lcd, FBIOGET_VSCREENINFO, &finfo) != 0)
    {
        perror("获取LCD硬件参数信息失败");
        return;
    }

    p->width = finfo.xres;
    p->height= finfo.yres;
    p->bpp   = finfo.bits_per_pixel;

    int screen_size = finfo.xres*finfo.yres*finfo.bits_per_pixel/8;
    p->fbm = mmap(NULL, screen_size * 2,
                  PROT_WRITE, MAP_SHARED, p->lcd, 0);
    if(p->fbm == MAP_FAILED)
    {
        perror("内存映射失败");
        return;
    }
    bzero(p->fbm, screen_size * 2);

    // 设定可见区的位置：设定A区为可见区
    finfo.xoffset = 0;
    finfo.yoffset = 0;
    ioctl(p->lcd, FBIOPAN_DISPLAY, &finfo);

    initialized = true;
}

void release_lcd(struct lcd_info *p)
{
    if(p == NULL)
        return;
    
    munmap(p->fbm, p->width*p->height*p->bpp/8);
    close(p->lcd);
}