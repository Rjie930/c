#include <stdio.h>
#include <dlfcn.h>  // 动态加载动态库的头文件：dlopen()、dlsym()
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>

// BMP格式头规范
struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

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
}__attribute__((packed));

// 以下结构体不一定存在于BMP文件中，除非：
// bitmap_info.compression为真
struct rgb_quad
{
	int8_t blue;
	int8_t green;
	int8_t red;
	int8_t reserved;
}__attribute__((packed));

int main(void)
{
	int fd = open("1.bmp", O_RDONLY);

	// 读取BMP格式头，获取图片信息
	struct bitmap_header header;
	struct bitmap_info info;
	struct rgb_quad quad;

    // 第一、二个结构体是必有信息
	read(fd, &header, sizeof(header));
	read(fd, &info, sizeof(info));

    // 第三个结构体是可选的，需要判断是否存在
	if(info.compression != 0)
	{
		read(fd, &quad, sizeof(quad));
		fprintf(stderr, "read quad! \n");
	}

    // 输出图片相关信息
    printf("图片大小: %d\n", header.size);
    printf("图片尺寸: %d×%d\n", info.width, info.height);
    printf("色深: %dbpp\n", info.bit_count);
    
    close(fd);
    return 0;
}