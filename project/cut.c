/*******************************************************************
 * @Descripttion   : Ctrl+C -> Ctrl+V
 * @version        : 3.14
 * @Author         : Rjie
 * @Date           : 2023-09-05 20:22
 * @LastEditTime   : 2023-09-06 09:39
 *******************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define piece_len 500 * 1024 * 1024

void merger()
{
    char *buf = calloc(1, piece_len);
    char file_out[20];
    int end, cur, ret = 1;
    FILE *fp, *fp_out;
    fp = fopen("file_merger", "w+");
    if (fp_out == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0;; i++)
    {
        sprintf(file_out, "%d", i);
        fp_out = fopen(file_out, "r");
        if (fp_out == NULL)
        {
            printf("done!\n");
            break;
        }
        bzero(buf, piece_len);
        ret = fread(buf, piece_len, 1, fp_out);
        printf("ret = %d\n", ret);
        if (ret < 1)
        {
            if (ferror(fp_out))
            {
                perror("");
                exit(1);
            }
            else if (feof(fp_out))
            {
                end = ftell(fp_out);
                fwrite(buf, end, 1, fp);
            }
        }
        else
        {
            fwrite(buf, ftell(fp_out), 1, fp);
        }
        fclose(fp_out);
    }
    free(buf);
    fclose(fp);
}

void cut(const char *file)
{
    // 1. 打开文件
    // char *file = "1.png";
    FILE *fp = fopen(file, "rw+");
    if (fp == NULL)
        perror("open file error");

    // 2. 读取文件大小
    fseek(fp, 0, SEEK_END);
    int file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 3. 分块数
    int block = file_len / (piece_len) + ((file_len % piece_len > 0) ? 1 : 0);

    printf("file_len = %d\n", file_len);
    printf("block = %d\n", block);

    // 4. 读取文件
    char *buf = calloc(1, piece_len);
    FILE *fp_out;
    char file_out[20];
    int end, cur, ret = 1;
    for (int i = 0; ret > 0; i++)
    {
        bzero(buf, piece_len);
        sprintf(file_out, "%d", i);
        fp_out = fopen(file_out, "w+");
        cur = ftell(fp);
        ret = fread(buf, piece_len, 1, fp);
        printf("ret = %d\n", ret);
        if (ret < 1)
        {
            if (ferror(fp))
            {
                perror("");
                exit(1);
            }
            else if (feof(fp))
            {
                end = ftell(fp);
                fwrite(buf, end - cur, 1, fp_out);
                break;
            }
        }
        fwrite(buf, piece_len, 1, fp_out);
    }
    fclose(fp_out);
    fclose(fp);
}

int main(int argc, char const *argv[])
{
    if (argv[1] == NULL)
    {
        merger();
    }
    else
    {
        cut(argv[1]);
    }
    return 0;
}