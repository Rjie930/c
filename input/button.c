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
#include "jpeglib.h"
#include <sys/stat.h>
#include <stdbool.h>

char *jpg2rgb(const char *jpgdata, size_t jpgsize, int *img_width, int *img_height, int *img_bpp)
{
    // 1，声明解码结构体，以及错误管理结构体
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // 2，使用缺省的出错处理来初始化解码结构体
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // 3，配置该cinfo，使其从 jpgdata 中读取jpgsize个字节
    //    这些数据必须是完整的JPEG数据
    jpeg_mem_src(&cinfo, jpgdata, jpgsize);

    // 4，读取JPEG文件的头，并判断其格式是否合法
    if (!jpeg_read_header(&cinfo, true))
    {
        fprintf(stderr, "jpeg_read_header failed: "
                        "%s\n",
                strerror(errno));
        return NULL;
    }

    // 5，开始解码
    jpeg_start_decompress(&cinfo);

    // 6，获取图片的尺寸信息
    printf("宽：  %d\n", *img_width = cinfo.output_width);
    printf("高：  %d\n", *img_height = cinfo.output_height);
    printf("色深：%d\n", *img_bpp = cinfo.output_components);

    // 7，根据图片的尺寸大小，分配一块相应的内存rgbdata
    //    用来存放从jpgdata解码出来的图像数据
    unsigned long linesize = cinfo.output_width * cinfo.output_components;
    unsigned long rgbsize = linesize * cinfo.output_height;
    char *rgbdata = calloc(1, rgbsize);

    // 8，循环地将图片的每一行读出并解码到rgb_buffer中
    int line = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        unsigned char *buffer_array[1];
        buffer_array[0] = rgbdata + cinfo.output_scanline * linesize;
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }

    // 9，解码完了，将jpeg相关的资源释放掉
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return rgbdata;
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

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    FILE *fp = fopen("on.jpg", "r");
    FILE *fp1 = fopen("off.jpg", "r");
    struct stat jpgInfo;
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

    stat("on.jpg", &jpgInfo);
    int on_size = jpgInfo.st_size;
    char *jpg_on = calloc(1, on_size);

    stat("off.jpg", &jpgInfo);
    int off_size = jpgInfo.st_size;
    char *jpg_off = calloc(1, off_size);

    while (1)
    {
        int n1 = fread(jpg_off, off_size, 1, fp1);
        int n = fread(jpg_on, on_size, 1, fp);
        if (n == 0 && n1 == 0)
        {
            printf("read jpg done\n");
            break;
        }
    }

    int img_width;
    int img_height;
    int img_bpp;

    char *on_RGB = jpg2rgb(jpg_on, on_size, &img_width, &img_height, &img_bpp);
    char *p = mmap(NULL, 800 * 480 * 4 * 2, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);

    int lcd_offset;
    int rgb_offset;
    for (int j = 0; j < img_height && j < 480; j++)
    {
        lcd_offset = 800 * 4 * j;
        rgb_offset = (img_width * img_bpp) * j;
        for (int i = 0; i < img_width && i < 800; i++)
        {
            memcpy(p + lcd_offset + 0 + 4 * i, on_RGB + rgb_offset + 2 + 3 * i, 1);
            memcpy(p + lcd_offset + 1 + 4 * i, on_RGB + rgb_offset + 1 + 3 * i, 1);
            memcpy(p + lcd_offset + 2 + 4 * i, on_RGB + rgb_offset + 0 + 3 * i, 1);
        }
    }

    char *off_RGB = jpg2rgb(jpg_off, off_size, &img_width, &img_height, &img_bpp);
    for (int j = 0; j < img_height && j < 480; j++)
    {
        lcd_offset = 800 * 4 * j + 480 * 800 * 4;
        rgb_offset = (img_width * img_bpp) * j;
        for (int i = 0; i < img_width && i < 800; i++)
        {
            memcpy(p + lcd_offset + 0 + 4 * i, off_RGB + rgb_offset + 2 + 3 * i, 1);
            memcpy(p + lcd_offset + 1 + 4 * i, off_RGB + rgb_offset + 1 + 3 * i, 1);
            memcpy(p + lcd_offset + 2 + 4 * i, off_RGB + rgb_offset + 0 + 3 * i, 1);
        }
    }

    while (1)
    {
        Touch_off();
        varinfo.yoffset = 0;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);

        Touch_on();
        varinfo.yoffset = 480;
        ioctl(lcd, FBIOPAN_DISPLAY, &varinfo);
    }
    close(lcd);
    fclose(fp);
    fclose(fp1);
    munmap(p, 800 * 400 * 4 * 2);
    return 0;
}
