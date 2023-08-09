/*
 * @Author: vincent
 * @Date: 2023-08-06 23:17:26
 * @LastEditTime: 2023-08-07 00:51:04
 */
#ifndef __HEAD_H
#define __HEAD_H

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <dlfcn.h>


extern struct fb_var_screeninfo finfo;

// LCD相关信息结构体
struct lcd_info
{
    int lcd; // 文件描述符
    int width;
    int height;
    int bpp; // bits per pixel

    char *fbm; // 显存入口
};

void init_lcd(struct lcd_info *p);
void release_lcd(struct lcd_info *p);



// 图像相关的信息
struct img_info
{
    FILE *fp;
    int width;
    int height;
    int bpp;
    int img_size; // 原始图片大小

    char *RGB;
};

char * load_img(const char *jpgFile, struct img_info *p);
void jpg2rgb(const char *jpgdata, size_t jpgsize, struct img_info *p);
void showImage(const char *jpgFile, struct lcd_info *p);

#endif
