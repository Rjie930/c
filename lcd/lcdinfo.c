#include <linux/fb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int lcd;
struct fb_fix_screeninfo fixinfo;
struct fb_var_screeninfo varinfo;

int main(int argc, char const *argv[])
{
    lcd = open("/dev/fb0",O_RDWR);
    perror("");

    
    return 0;
}
