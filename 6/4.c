#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void str_cat(char *a, char*b)
{
    char str[strlen(a)+strlen(b)+1];
    for(int i=0;i<strlen(a);i++)
    {
        str[i]=a[i];
    }
    for(int i=0;i<strlen(b);i++)
    {
        str[i+strlen(a)]=b[i];
    }
    str[strlen(a)+strlen(b)]='\0';
    printf("%s\n", str);
}
void main()
{
    char a[]= "123";
    char b[]= "456";
    str_cat(a,b);
}
