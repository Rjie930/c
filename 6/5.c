/*******************************************************************
 * @Descripttion   : 
 * @version        : 
 * @Author         : Rjie
 * @Date           : 2023-07-19 20:13
 * @LastEditTime   : 2023-07-20 11:23
*******************************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int fibonaci(int num)
{
    int sum;
    if(num<3)
    return 1;
    sum=fibonaci(num-1)+fibonaci(num-2);
    return sum;
}
void main()
{
    printf("%d\n",fibonaci(9));
}
