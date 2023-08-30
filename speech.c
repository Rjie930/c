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
    
    int *m=malloc(4);
    int *c=calloc(1,4);
    printf("m:%d\tc:%d\n\n",*m,*c);

    char *str=malloc(100);
    strcpy(str,"abc");

    printf("str:%s\tadress:%p\n\n",str,str);

    str=realloc(str,101);
    printf("realloc后：%s\tadress:%p\n",str,str);

    // free(str);
    // str=NULL;
    // long  i=(long)str;
    // printf("%ld,%s\n",i,(char*)i);
}
