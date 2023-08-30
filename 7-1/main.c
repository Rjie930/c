/*
 * @Author: vincent
 * @Date: 2023-08-05 09:03:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-07 01:37:51
 * @Description: 
 */

#include "head.h"

int main(int argc, char const *argv[]) // ./a.out xxx.jpg/xxx.bmp
{
    if(argc != 2)
    {
        fprintf(stderr, "请指定一张图片\n");
        exit(0);
    }

    // 1. 准备好LCD设备
    struct lcd_info linfo; // 显存入口、分辨率……
    init_lcd(&linfo);
    
    // 打开指定的动态库，并找到显示图片的函数指针
    void *handle;
    if(strstr(argv[1], ".jpg"))
        handle = dlopen("libjpg.so", RTLD_NOW);
    else if(strstr(argv[1], ".bmp"))
        handle = dlopen("libbmp.so", RTLD_NOW);

    if(handle == NULL)
    {
        fprintf(stderr, "打开动态库失败: %s\n", dlerror());
        exit(0);
    }

    void (*showImage)(const char *jpgFile, struct lcd_info *p);
    showImage = dlsym(handle, "showImage");
    if(showImage == NULL)
    {
        fprintf(stderr, "dlsym()失败: %s\n", dlerror());
        exit(0);
    }

    // 2. 显示BMP/JPG图片
    showImage(argv[1], &linfo);

    // 3. 释放相关资源
    release_lcd(&linfo);

    return 0;
}
