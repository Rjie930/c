#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void main()
{
    int N=2;
    float X=3.14;
    float sum=0;

    for(int i=0;i*i<N*N;i++)
    {
        sum=X*X;
    }
    if(N<0)
    sum=1.0/sum;
    printf("%f\n", sum);
}
