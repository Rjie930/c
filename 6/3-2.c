#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

float mypow(int N,float X)
{
    if(N>1)
    mypow(--N,X);
    return X*X;
}

void main()
{
    int N=-2;
    float X=3.14;
    float sum=0;
    sum=mypow(N,X);
    if(N<0)
    sum=1.0/sum;
    printf("%f\n", sum);
}
