#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char **argv)
{
    struct stat info; 
    int size=0;
    char chr;
    FILE *fp = fopen("file", "r+");
    if (fp == NULL)
    {
        printf("打开文件%s失败%s\n", argv[1], strerror(errno));
        exit(0);
    }
    while(fread(&chr,1,1,fp))
    size++;
    
    stat("file",&info);
    fseek(fp,0,SEEK_END);

    printf("file size:%d\n",size);
    printf("stat file size:%ld\n",info.st_size);
    printf("fseek file size:%ld\n",ftell(fp));
    fclose(fp);
}