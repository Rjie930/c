
#include "head.h"
#include "jpeglib.h"

/**
 * @description: 加载指定jpeg图片，并获取jpeg尺寸
 * @param {char} *jpgFile: 图片名称
 * @param {int} *pjpgsize: 图片大小
 * @return {char *} 返回JPG图片数据
 */
char * load_img(const char *jpgFile, struct img_info *p)
{
    if(jpgFile == NULL)
        return NULL;

    p->fp = fopen(jpgFile, "r");
    if(p->fp == NULL)
    {
        perror("打开jpg图片失败");
        return NULL;
    }

    fseek(p->fp, 0, SEEK_END);
    p->img_size = ftell(p->fp);
    fseek(p->fp, 0, SEEK_SET);

    char *jpgdata = calloc(1, p->img_size);

    while(1)
    {
        int n = fread(jpgdata, p->img_size, 1, p->fp);

        if(n == 1)
            continue;

        if(n < 1)
        {
            if(feof(p->fp))
                break;

            if(ferror(p->fp))
            {
                perror("加载图片失败");
                fclose(p->fp);
                free(jpgdata);
                return NULL;
            }
        }
    }

    return jpgdata;
}

/**
 * @description: 将指定jpg数据解码为RGB数据，并获取其尺寸信息
 * @param {char} *jpgdata : jpg图片数据
 * @param {size_t} jpgsize: jpg图片数据大小
 * @param {img_info} *p   : 图片信息结构体指针
 * @return {} : 无
 */
void jpg2rgb(const char *jpgdata, size_t jpgsize, struct img_info *p)
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
    if(!jpeg_read_header(&cinfo, true))
    {
        fprintf(stderr, "jpeg_read_header failed: "
            "%s\n", strerror(errno));
        return;
    }

    // 5，开始解码
    jpeg_start_decompress(&cinfo);

    // 6，获取图片的尺寸信息
    p->width = cinfo.output_width;
    p->height= cinfo.output_height;
    p->bpp   = cinfo.output_components*8;

    // 7，根据图片的尺寸大小，分配一块相应的内存rgbdata
    //    用来存放从jpgdata解码出来的图像数据
    unsigned long linesize = cinfo.output_width * cinfo.output_components;
    unsigned long rgbsize  = linesize * cinfo.output_height;
    p->RGB = calloc(1, rgbsize);

    // 8，循环地将图片的每一行读出并解码到rgb_buffer中
    int line = 0;
    while(cinfo.output_scanline < cinfo.output_height)
    {
        unsigned char *buffer_array[1];
        buffer_array[0] = p->RGB + cinfo.output_scanline * linesize;
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }

    // 9，解码完了，将jpeg相关的资源释放掉
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return;
}


void showImage(const char *jpgFile, struct lcd_info *p)
{
    // 0. 定义图片信息结构体
    struct img_info minfo;

    // 1. 读取JPG图片
    char *jpgdata = load_img(jpgFile, &minfo);

    // 2. 将JPG解码为RGB
    jpg2rgb(jpgdata, minfo.img_size, &minfo);
    printf("jgpsize: %d\n", minfo.img_size);
    printf("jgp 尺寸: %d×%d\n", minfo.width, minfo.height);

    // 计算缩小（若需）后的图片的尺寸
    int mwidth = minfo.width;
    int mheight= minfo.height;
    int lwidth = p->width;
    int lheight= p->height;
    bool shrink = false;
    if(minfo.width > p->width || minfo.height > p->height)
    {
        mwidth  /= 2;
        mheight /= 2;
        lwidth  *= 2;
        lheight *= 2;
        shrink = true;
    }

    // 3. 将显存入口指针调整到居中位置（x,y）
    int x = (800-mwidth)/2;
    int y = (480-mheight)/2;
    char *tmp = p->fbm + p->width*p->bpp/8 * y + p->bpp/8 * x;

    // 4. 妥善地将RGB放置到显存中
    // static int n = 1;
    int screen_size = p->width * p->bpp/8 * p->height;
    // tmp += screen_size * (n%2);
    for(int lj=0, mj=0; mj<minfo.height && lj<lheight-y; shrink?mj+=2:mj++, lj++)
    {
        int moffset = minfo.width*minfo.bpp/8 * mj;
        int loffset = p->width*p->bpp/8 * lj;

        for(int li=0, mi=0; mi<minfo.width && li<lwidth-x; shrink?mi+=2:mi++, li++)
        {
            memcpy(tmp+4*li + loffset+0, minfo.RGB+3*mi + moffset+2, 1);
            memcpy(tmp+4*li + loffset+1, minfo.RGB+3*mi + moffset+1, 1);
            memcpy(tmp+4*li + loffset+2, minfo.RGB+3*mi + moffset+0, 1);
        }
    }

    // 将可见区设定为另一边
    // finfo.xoffset = 0;
    // finfo.yoffset = 480*(n%2);
    // ioctl(p->lcd, FBIOPAN_DISPLAY, &finfo);
    // n++;

    // 5. 释放相关资源
    fclose(minfo.fp);
    free(minfo.RGB);
}