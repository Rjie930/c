/*
 * @Author: vincent
 * @Date: 2023-08-01 10:23:59
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-01 10:55:08
 * @Description: 
 */
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    // 1. 打开times.txt文件
    FILE *fp_time = fopen("times.txt", "a"); // 只写（追加）

    // 2. 打开line文件（需判定文件是否存在）
    int num = 0;
    FILE *fp_line;
    if(access("line", F_OK) == 0) // 文件存在
    {
        fp_line = fopen("line", "r+"); // 文件必须存在
        fread(&num, sizeof(int), 1, fp_line);
    }
    else // 文件不存在
    {
        fp_line = fopen("line", "w+"); // 创建、清空
        num = 0;
    }

    // 3. 将序号和系统时间不断写入times.txt文件
    for(;;) // while(1)
    {
        // a. 将行号写入
        fprintf(fp_time, "%d, ", ++num);

        // b. 将时间写入
        time_t t = time(NULL);
        fprintf(fp_time, "%s", ctime(&t));

        // c. 更新line文件中的序号
        fseek(fp_line, 0L, SEEK_SET); // 文件位置移到开头处，覆盖原有内容
        fwrite(&num, sizeof(int), 1, fp_line);

        fflush(fp_time);
        fflush(fp_line);
        sleep(1);
    }

    fclose(fp_time);
    fclose(fp_line);

    return 0;
}
