#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>

// #include <cjson/cJSON.h>
void show_png(char *path,int x,int y);
void main(int argc,char **argv)
{
    for(int x=0,y=100;y<480;)
    for(;x<600;x+=200,y+=100){
    show_png(argv[1], x, y);
    usleep(100000);}
    // char buf[17*512];
    // char buf_wea[100];
    // int fd=open("wea.json",O_RDWR);
    // int offset = lseek(fd,0,SEEK_END);
    // lseek(fd,0,SEEK_SET);
    // read(fd,buf,offset);
    // // printf("%d\n",offset);

    // // size_t realsize = size * nmemb;
    // // printf("%s\n", (char *)contents);
    // cJSON *json = cJSON_Parse(buf);
    // cJSON *city =cJSON_GetObjectItem(json, "city");
    // printf("city:%s\n",city->valuestring);

    // cJSON *wea =cJSON_GetObjectItem(json, "wea");
    // printf("wea:%s\n",wea->valuestring);

    // cJSON *tem =cJSON_GetObjectItem(json, "tem");
    // printf("tem:%s\n",tem->valuestring);

    // cJSON *humidity =cJSON_GetObjectItem(json, "humidity");
    // printf("humidity:%s\n",humidity->valuestring);

    // cJSON *win =cJSON_GetObjectItem(json, "win");
    // printf("win:%s\n",win->valuestring);

    // cJSON *win_speed =cJSON_GetObjectItem(json, "win_speed");
    // printf("win_speed:%s\n",win_speed->valuestring);

    // cJSON *air_tips =cJSON_GetObjectItem(json, "air_tips");
    // printf("air_tips:%s\n",air_tips->valuestring);

// char a[20];
// char b[20];
// char c[20];
// char d[20];
    // sprintf(buf_wea,"%s:%s\n温度:%s°,湿度:%s,%s,%s\n%s",city->valuestring ,wea->valuestring ,tem->valuestring, humidity->valuestring ,win->valuestring ,win_speed->valuestring, air_tips->valuestring);
// char buf_wea[600]="生活小贴士:各类人群可多参加户外活动，多呼吸一下清新的空气。\r";
//     sscanf(buf_wea,"city:%[^\r]:%stem:%shumidity:%s%",a,b,c,d);
//         printf("1%s\n2%s\n3%s\n4%s\n", a,b,c,d);
//         printf("%d\n", strlen(buf_wea));
    // if (json)
    // {
    //     cJSON *data = cJSON_GetObjectItem(json, "data");
    //     if (data)
    //     {
    //         cJSON *city = cJSON_GetObjectItem(data, "city");
    //         cJSON *wea = cJSON_GetObjectItem(data, "wea");

    //         if (city && wea)
    //         {
    //             printf("城市: %s\n天气: %s\n", city->valuestring, wea->valuestring);
    //         }
    //         else
    //         {
    //             printf("解析JSON出错\n");
    //         }
    //     }

    //     cJSON_Delete(json);
    // }
}
