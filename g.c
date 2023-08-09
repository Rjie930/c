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
#include <fcntl.h>

#include "font.h"

void showbitmap(bitmap *bm, int x, int y)
{
    // 准备好LCD设备（如果多次显示，应该只做一遍）

    // 字体的信息：
    bm->width;
    bm->height;
    bm->byteperpixel;
    bm->map; // 指向RGB数据，没有上下颠倒，也没有一行4字节倍数的约束，就是纯粹的RGB
}

int main(int argc, char const *argv[])
{
    // 1.初始化字库
    //  注意要先将ttf文件放入开发板上
    font *f = fontLoad("simfang.ttf"); // 指定字库文件，比如simfang.ttf

    // 2.设置字体的大小
    fontSetSize(f, 24);

    // 3.设置字体输出框的大小
    bitmap *bm;
    bm = createBitmapWithInit(800, 28, 4, 0xFFFFFFFF);

    // 4.把字体输出到输出框中
    fontPrint(f, bm, 80, 2, "北京天安门", 0xFF000000, 0);

    // 5.将bm妥善地放置到LCD上显示出来
    showbitmap(bm, 0, 0);

    return 0;
}
