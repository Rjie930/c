/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-08-08 20:20
 * @LastEditTime   : 2023-08-08 20:20
*******************************************************************/
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>

void show_image(char const *image_path);

int main(int argc, char const *argv[])
{
    void *handle = NULL;
    if (strstr(argv[1], ".bmp"))
    {
        printf("libbmp.so\n");
        handle = dlopen("libbmp.so", RTLD_NOW);
    }
    else if (strstr(argv[1], ".jpg"))
    {
        printf("libjpg.so\n");
        handle = dlopen("libjpg.so", RTLD_NOW);
    }
    else
    {
        printf("fail %s\n",dlerror());
        exit(0);
    }
    if(handle==NULL)
    printf("handle %s\n",dlerror());
    void (*show_image)(char const *image_path);
    show_image = dlsym(handle, "show_image");
    if (show_image == NULL)
    {
        printf("查找函数失败\n");
        exit(0);
    }
    show_image(argv[1]);
}
