#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <png.h>

void show_png(char *path, int x, int y)
{
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1)
    {
        perror("打开lcd失败");
        exit(0);
    }

    int *p = mmap(NULL, 800 * 480 * 4, PROT_WRITE, MAP_SHARED, lcd, 0);
    if (p == MAP_FAILED)
    {
        perror("内存映射失败");
        exit(0);
    }

    int width = 800;
    int height = 480;

    // Open the PNG file
    FILE *png_file = fopen(path, "rb");
    if (!png_file)
    {
        perror("无法打开PNG文件");
        exit(0);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        perror("PNG解码错误");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        exit(0);
    }

    // Initialize PNG I/O
    png_init_io(png_ptr, png_file);
    png_read_info(png_ptr, info_ptr);

    // Get image information
    png_uint_32 png_width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 png_height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Read the image data
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * png_height);
    for (int y = 0; y < png_height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, row_pointers);

    p += 800 * y + x;
    // Copy the image data to the screen buffer, setting transparent areas to black
    for (int j = 0; j < png_height; j++)
    {
        for (int i = 0; i < png_width; i++)
        {
            png_bytep pixel = &(row_pointers[j][i * 4]);
            int color = (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
            int alpha = pixel[3];

            if (alpha == 0)
            {
                // Set transparent areas to black
                p[i + width * j] = 0; // Black color (0x00000000)
            }
            else
            {
                // Combine with existing screen buffer color using alpha
                int blended_color = color;
                p[i + width * j] = blended_color;
            }
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(png_file);
    for (int y = 0; y < png_height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    close(lcd);
    munmap(p, 800 * 480 * 4);
}
// int main(int argc, char const *argv[])
// {
//     int lcd = open("/dev/fb0", O_RDWR);
//     char *p = mmap(NULL, 800 * 480 * 4, PROT_WRITE | PROT_READ, MAP_SHARED, lcd, 0);
//     show_png("yahoo/lei.png",0,0);
//     close(lcd);
//     munmap(p, 800 * 400 * 4);
//     return 0;
// }