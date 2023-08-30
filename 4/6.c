#include <stdio.h>
void main()
{
    char a[]="abccdeeffg";
    printf("前:%s\n",a);
    int len=sizeof(a);
    for(int i=0;i<len-1;i++)
    {
        if(*(a+i)==*(a+i+1))
        for(int j=i+1;j<len-1;j++)
        *(a+j)=*(a+j+1);
    }
    printf("后:%s\n",a);
}
