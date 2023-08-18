#include <linux/fb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

struct bitmap_header
{
    int16_t type;
    int32_t size; // 图像文件大小
    int16_t reserved1;
    int16_t reserved2;
    int32_t offbits; // bmp图像数据偏移量
} __attribute__((packed));

struct bitmap_info
{
    int32_t size;   // 本结构大小
    int32_t width;  // 图像宽
    int32_t height; // 图像高
    int16_t planes;

    int16_t bit_count; // 色深
    int32_t compression;
    int32_t size_img; // bmp数据大小，必须是4的整数倍
    int32_t X_pel;
    int32_t Y_pel;
    int32_t clrused;
    int32_t clrImportant;
} __attribute__((packed));

// 以下结构体不一定存在于BMP文件中，除非：
// bitmap_info.compression为真
struct rgb_quad
{
    int8_t blue;
    int8_t green;
    int8_t red;
    int8_t reserved;
} __attribute__((packed));

void show_bmp(char const * image_path,char *p,int xyoffset)
{
    p+=800*480*4*xyoffset;
    FILE *fp = fopen(image_path, "r");
    perror("");
    struct bitmap_header head;
    struct bitmap_info info;
    struct rgb_quad quad;

    fread(&head, sizeof(head), 1, fp);
    fread(&info, sizeof(info), 1, fp);
    if (info.compression)
        fread(&quad, sizeof(quad), 1, fp);
    int width = info.width;
    int height = info.height;
    int bpp = info.bit_count;

    int pad = (4 - (width * bpp / 8) % 4) % 4;

    char *rgb = calloc(1, (width * bpp / 8 + pad) * height);

    fread(rgb, (width * bpp / 8 + pad) * height, 1, fp);

    char *tmp = rgb + (width * bpp / 8 + pad) * (height - 1);
    int lcd_offset = 0;
    int rgb_offset = 0;
    int zoom = 1;
    if (height > 480 || width > 800)
        zoom = (height / 480 + 1 > width / 800 + 1) ? height / 480 + 1 : width / 800 + 1;
    printf("%d\n", zoom);

    for (int j = 0; j < height && j < height / zoom; j++)
    {
        lcd_offset = 800 * 4 * j + (480 - height / zoom) / 2 * 800 * 4 + (800 - width / zoom) * 2;
        rgb_offset = (width * bpp / 8 + pad) * j * zoom;
        for (int i = 0; i < width && i < width / zoom; i++)
            memcpy(p + lcd_offset + 4 * i, tmp - rgb_offset + 3 * i * zoom, 3);
    }
    fclose(fp);
    free(rgb);
}
