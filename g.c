#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    printf("%s\n",(char *)contents);
    cJSON *json = cJSON_Parse((char *)contents);
    
    if (json) {
    cJSON *city =cJSON_GetObjectItem(json, "city");
    printf("city:%s\n",city->valuestring);

    cJSON *wea =cJSON_GetObjectItem(json, "wea");
    printf("wea:%s\n",wea->valuestring);

    cJSON *tem =cJSON_GetObjectItem(json, "tem");
    printf("tem:%s\n",tem->valuestring);

    cJSON *humidity =cJSON_GetObjectItem(json, "humidity");
    printf("humidity:%s\n",humidity->valuestring);

    cJSON *win =cJSON_GetObjectItem(json, "win");
    printf("win:%s\n",win->valuestring);

    cJSON *win_speed =cJSON_GetObjectItem(json, "win_speed");
    printf("win_speed:%s\n",win_speed->valuestring);

    cJSON *air_tips =cJSON_GetObjectItem(json, "air_tips");
    printf("air_tips:%s\n",air_tips->valuestring);
        
        cJSON_Delete(json);
    }
    
    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[] = "https://v0.yiketianqi.com/api?unescape=1&version=v63&appid=13833269&appsecret=23AJ0ebz";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
