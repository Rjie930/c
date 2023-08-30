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
    FILE *fp = fopen("time.txt", "a+");
    if (fp == NULL)
    {
        printf("打开文件%s失败%s\n", argv[1], strerror(errno));
        exit(0);
    }

    FILE *fp1 = fopen("line", "a+");
    if (fp1 == NULL)
    {
        printf("打开文件%s失败%s\n", argv[1], strerror(errno));
        exit(0);
    }
    int line=0;
    time_t cur;
    char chr;
    puts("123\n");
    fseek(fp1,0,SEEK_END);
    while(getc(fp1)!='\n')
    {
        printf("tell:%ld\n", ftell(fp1));
        fseek(fp1,-2,SEEK_CUR);
        // fread(&chr,1,1,fp1);
        // putchar(chr);

        usleep(1000000);
    }
    
    
    fscanf(fp1,"%d",&line);
    setvbuf(fp1,NULL,_IONBF,0);
    setvbuf(fp,NULL,_IONBF,0);
    while (1)
    {
        time(&cur);
        fprintf(fp1,"%d\n",++line);
        printf("%d,%s\n",line,ctime(&cur));
        fprintf(fp,"%d,%s\n",line,ctime(&cur));

        sleep(1);
    }
    printf("buf_size ");
    fclose(fp);
}