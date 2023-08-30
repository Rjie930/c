#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    static int x, y;
    int tp = open("/dev/input/event0", O_RDWR);
    if (tp == -1)
    {
        perror("open faile");
        exit(0);
    }

    struct input_event buf;

    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(tp, &buf, sizeof(buf));
        printf("type:%d\n",buf.type);
        if (buf.type == EV_ABS)
        {
            if (buf.code == ABS_X)
            {
                x = buf.value;
            }
            if (buf.code == ABS_Y)
            {
                y = buf.value;
            }
            printf("x:%d\ny:%d\n", x, y);
        }
        else if (buf.type == EV_KEY)
        {
            
            if (buf.code == BTN_TOUCH)
            {
                if (buf.value > 0)
                {
                    printf("按下\n");
                }
                else
                {
                    printf("松开\n");
                }
            }
        }
        else if (buf.type == EV_SYN)
        {
            printf("---------syn---------\n");
        }
    }
    return 0;
}
