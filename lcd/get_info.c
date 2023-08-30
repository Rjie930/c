#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int lcd;
struct fb_fix_screeninfo fixinfo;
struct fb_var_screeninfo varinfo;

int main(int argc, char const *argv[])
{
    lcd = open("/dev/fb0",O_RDWR);
    if (lcd==-1)
    {
        perror("open lcd fail");
        exit(0);
    }
    
    if(ioctl(lcd,FBIOGET_FSCREENINFO,&fixinfo)!=0)
    {
        perror("get fix info fail");
        return -1;
    }
    
    if(ioctl(lcd,FBIOGET_VSCREENINFO,&varinfo)!=0)
    {
        perror("get var info fail");
        return -1;
    }

    printf("x:%d,y:%d\n",varinfo.xres,varinfo.yres);
    printf("v_x:%d,v_y:%d\n",varinfo.xres_virtual,varinfo.yres_virtual);
    printf("bits: %d\n",varinfo.bits_per_pixel);

    return 0;
}
