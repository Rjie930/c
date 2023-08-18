#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <png.h>
#include <string.h>

void show_png(char *path, char *p, int xyoffset)
{
    int width = 800;
    int height = 480;

    FILE *png_file = fopen(path, "rb");
    if (!png_file)
    {
        perror("无法打开PNG文件");
        exit(0);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        perror("PNG解码错误");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        exit(0);
    }

    png_init_io(png_ptr, png_file);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 png_width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 png_height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * png_height);
    for (int y = 0; y < png_height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, row_pointers);
    int bg = 0x0;
    int x = (800 - png_width) / 2;
    int y = (480 - png_height) / 2;
    p += 800 * 480 * 4 * xyoffset;
    for (int j = 0; j < 480; j++)
        for (int i = 0; i < x; i++)
            memcpy(p + i * 4 + width * 4 * j, &bg, 4);

    for (int j = 0; j < y; j++)
        for (int i = 0; i < 800; i++)
            memcpy(p + i * 4 + width * 4 * j, &bg, 4);

    p += 800 * 4 * y + x * 4;
    for (int j = 0; j < height - y; j++)
    {
        for (int i = 0; i < width - x; i++)
        {
            if (j < png_height && i < png_width)
            {
                png_bytep pixel = &(row_pointers[j][i * 4]);
                int color = (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
                int alpha = pixel[3];
                if (alpha == 0)
                {
                    // p[i + width * j] = 0; // Black color (0x00000000)
                    memcpy(p + i * 4 + width * 4 * j, &bg, 4);
                }
                else
                {
                    // p[i*4 + width *4* j] = blended_color;
                    memcpy(p + i * 4 + width * 4 * j, &color, 4);
                }
            }
            else
            {
                memcpy(p + i * 4 + width * 4 * j, &bg, 4);
            }
        }
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(png_file);
    for (int y = 0; y < png_height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);
}
// int main(int argc, char const *argv[])
// {
//     int lcd = open("/dev/fb0", O_RDWR);
//     char *p = mmap(NULL, 800 * 480 * 4*3, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);
//     show_png("yahoo/yu.png",p,0);
//     close(lcd);
//     munmap(p, 800 * 400 * 4);
//     return 0;
// }