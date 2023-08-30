#include <stdio.h>

void main(){

    int a=0x12345678;
    char *p=(char*)&a;
    printf("%#x\n",*p);
}