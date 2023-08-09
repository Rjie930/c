#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp = fopen("file1", "r+");
    if (fp == NULL)
    {
        printf("打开文件%s失败%s\n", argv[1], strerror(errno));
        exit(0);
    }
    FILE *fp1 = fopen("file4", "w+");
    if (fp1 == NULL)
    {
        printf("打开文件%s失败%s\n", argv[2], strerror(errno));
        exit(0);
    }
    char buf[20];
    int ret = 1;
    int end = 0, cur = 0;
    while (ret > 0)
    {
        cur = ftell(fp);
        bzero(buf, 20);
        ret = fread(buf, 10, 2, fp);
        printf("%d\n", ret);
        if (ret < 2)
        {
            if (ferror(fp))
                perror("read");
            else if (feof(fp))
            {
                end = ftell(fp);
                fwrite(buf, end - cur, 1, fp1);
            }
        }

        ret = fwrite(buf, 10, ret, fp1);
        if (ret < 1)
        {
            printf("done!\n");
            break;
        }
        printf("write:%d ", ret);
    }
    fclose(fp);
    fclose(fp1);
}