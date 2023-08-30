/*
 * @Author: vincent
 * @Date: 2023-08-06 23:15:57
 * @LastEditTime: 2023-08-07 00:36:58
 */
#include "head.h"
#include "bmp.h"

void showImage(const char *bmpFile, struct lcd_info *p)
{
    // 打开图片文件
    FILE *fp = fopen(bmpFile, "r");
    if(fp == NULL)
    {
        perror("打开图片失败");
        exit(0);
    }

    // 读取图片格式头，获取其尺寸信息
    struct bitmap_header head;
    struct bitmap_info   info;
    struct rgb_quad      quad;

    fread(&head, sizeof(head), 1, fp);
    fread(&info, sizeof(info), 1, fp);
    if(info.compression)
        fread(&quad, sizeof(quad), 1, fp);

    int mwidth = info.width;
    int mheight= info.height;
    int mbpp   = info.bit_count;

    int pad = (4-(mwidth*mbpp/8)%4)%4; // 每行末尾的无效数据的字节数
    char *rgb = calloc(1, (mwidth*mbpp/8+pad)*mheight);

    // 读取图片RGB内容
    while(1)
    {
        int n = fread(rgb, (mwidth*mbpp/8+pad)*mheight, 1, fp);

        if(n < 1) // 可能遇到文件尾，或者遇到错误
        {
            if(feof(fp))
            {
                break;
            }

            if(ferror(fp))
            {
                perror("读取图片数据失败");
                goto err;
            }
        }
    }

    // 计算缩小（若需）后的图片的尺寸
    int lwidth = p->width;
    int lheight= p->height;
    bool shrink = false;
    if(mwidth > p->width || mheight > p->height)
    {
        mwidth  /= 2;
        mheight /= 2;
        lwidth  *= 2;
        lheight *= 2;
        shrink = true;
    }

    // 让rgb指向图片的末行的首字节
    char *mtmp = rgb + (info.width*mbpp/8+pad)*(info.height-1);

    // 3. 将显存入口指针调整到居中位置（x,y）
    int x = (p->width-mwidth)/2;
    int y = (p->height-mheight)/2;
    char *ltmp = p->fbm + p->width*p->bpp/8 * y + p->bpp/8 * x;


    // 将rgb中的图像数据，妥善地拷贝到显存中
    int lcd_offset = 0;
    int rgb_offset = 0;

    for(int mj=0,lj=0; mj<info.height && lj<lheight-y; shrink?mj+=2:mj++, lj++)
    {
        // 让p指向下一行，rgb指向下一行
        lcd_offset = 800*4*lj;
        rgb_offset = (info.width*mbpp/8 + pad)*mj;

        // 从左到右显示一行图像
        for(int mi=0,li=0; mi<info.width && li<lwidth-x; shrink?mi+=2:mi++, li++)
        {
            memcpy( ltmp +4*li + lcd_offset,
                    mtmp +3*mi - rgb_offset, 3);
        }
    }

err:
    fclose(fp);
    munmap(p, 800*480*4);
    free(rgb);
}