#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include "jpeglib.h"
#include <sys/mman.h>

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

void show_image(char const *image_path)
{
    if (image_path == NULL)
    {
        printf("指定一张jpeg图片、你");
        exit(0);
    }

    int lcd = open("/dev/fb0", O_RDWR);
    FILE *fp = fopen(image_path, "r");

    struct stat jpgInfo;
    stat(image_path, &jpgInfo);

    char *jpgdata = calloc(1, jpgInfo.st_size);
    while (1)
    {
        int n = fread(jpgdata, jpgInfo.st_size, 1, fp);
        if (n == 1)
            break;
        if (n < 0)
        {
            if (feof(fp))
                break;
            if (ferror(fp))
            {
                perror("读取图片识别");
                exit(0);
            }
        }
    }
    int img_width;
    int img_height;
    int img_bpp;

    char *RGB = jpg2rgb(jpgdata, jpgInfo.st_size, &img_width, &img_height, &img_bpp);

    char *p = mmap(NULL, 800 * 480 * 4, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);
    int lcd_offset;
    int rgb_offset;
    int zoom = 1;
    if (img_height > 480 || img_width > 800)
        zoom = (img_height / 480 + 1 > img_width / 800 + 1) ? img_height / 480 + 1 : img_width / 800 + 1;
    for (int j = 0; j < img_height && j < img_height / zoom; j++)
    {
        lcd_offset = 800 * 4 * j+(480 - img_height / zoom) / 2 * 800 * 4 + (800 - img_width / zoom) * 2;
        rgb_offset = (img_width * img_bpp) * j*zoom;
        for (int i = 0; i < img_width && i < img_width / zoom; i++)
        {
            memcpy(p + lcd_offset + 0 + 4 * i, RGB + rgb_offset + 2 + 3 * i* zoom, 1);
            memcpy(p + lcd_offset + 1 + 4 * i, RGB + rgb_offset + 1 + 3 * i* zoom, 1);
            memcpy(p + lcd_offset + 2 + 4 * i, RGB + rgb_offset + 0 + 3 * i* zoom, 1);
        }
    }
    close(lcd);
    fclose(fp);
    munmap(p, 800 * 400 * 4);
}

// 参数说明:
//     fd: 文件描述符
//     buf: 数据缓冲区
//     count：希望读取的字节数
// 返回值:
//     失败: -1
//     成功: 读取的字节数
ssize_t Read(int fd, void *buf, size_t count)
{
    int total = 0;

    while(count > 0)
    {
        int n;
        while((n=read(fd, (char *)buf+total, count))==-1 &&
               errno == EINTR);

        if(n == -1) // 遇到了错误
        {
            perror("read失败");
            return -1;
        }

        if(n == 0) // 遇到了文件尾
            break;

        count -= n;
        total += n;
    }

    // 返回总共读到的字节数
    return total;
}


// 参数说明:
//     fd: 文件描述符
//     buf: 数据缓冲区
//     count: 希望写入的字节数
// 返回值:
//     失败: -1
//     成功: 写入的字节数
ssize_t Write(int fd, const void *buf, size_t count)
{
    int total = 0;
    while(count > 0)
    {
        int m;
        while((m=write(fd, buf+total, count))==-1 && errno==EINTR);

        if(m == -1)
        {
            perror("写入数据失败");
            return -1;
        }

        count -= m;
        total += m;
    }
    return total;
}