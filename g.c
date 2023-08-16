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
#include <stdio.h>
#include <pthread.h>

int main() {

    char *a=malloc(100);
    char *m=malloc(10);
    char *c=calloc(1,10);
    strcpy(a,"abc");

    printf("%s,%ld\n\n",a,a);
    a=realloc(a,400);
    printf("realloc后：%s,%ld\n",a,a);
    free(a);
    long long i=a;
    printf("%ld,%s\n",i,(char*)i);

}
