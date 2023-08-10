#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

char buf_send[600];
char buf_mesg[300];
char buf_wea[200];
char buf_tips[100];
void write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{

    cJSON *json = cJSON_Parse((char *)contents);

    if (json)
    {
        cJSON *city = cJSON_GetObjectItem(json, "city");
        cJSON *wea = cJSON_GetObjectItem(json, "wea");
        cJSON *tem = cJSON_GetObjectItem(json, "tem");
        cJSON *humidity = cJSON_GetObjectItem(json, "humidity");
        cJSON *win = cJSON_GetObjectItem(json, "win");
        cJSON *win_speed = cJSON_GetObjectItem(json, "win_speed");
        cJSON *air_tips = cJSON_GetObjectItem(json, "air_tips");

        sprintf(buf_wea,"%s:%s\n温度:%s度 湿度:%s\n风向:%s 风速:%s",city->valuestring ,wea->valuestring ,tem->valuestring, humidity->valuestring ,win->valuestring ,win_speed->valuestring);
        strcpy(buf_tips,air_tips->valuestring);
        printf("buf_wea:%s\n", buf_wea);
        printf("buf_tips:%s\n", buf_tips);

        cJSON_Delete(json);

    }
}

void getWea()
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        char url[] = "https://v0.yiketianqi.com/api?unescape=1&version=v63&appid=13833269&appsecret=23AJ0ebz";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

int main(int argc, char const *argv[])
{

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. 准备好对方的IP地址
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.16.134");
    addr.sin_port = htons(50001); // host to network short

    time_t rawTime;
    struct tm *timeInfo;
    char format[80];

    while (1)
    {
        bzero(buf_send, 100);
        fgets(buf_mesg, 300, stdin);
        time(&rawTime);
        getWea();
        printf("当前时间：%ld\n", rawTime);

        sprintf(buf_send, "%ld %s %s %s", rawTime, buf_mesg, buf_tips,buf_wea);
        sendto(sockfd, buf_send, strlen(buf_send), 0, (struct sockaddr *)&addr, sizeof(addr));
    }
    return 0;
}
