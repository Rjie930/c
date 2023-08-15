/*
 * @Author: vincent
 * @Date: 2023-08-15 10:40:09
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 10:52:52
 * @Description: 
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// #define DATATYPE ....
#include "stack.h"

int main(int argc, char const *argv[])
{
    // 1. 搞个空栈
    linkstack s = init_stack();

    int n;
    printf("输入十进制数：\n");
    scanf("%d",&n);
    while (n>0)
    {
        push(s,n%16);
        n/=16;
    }
    

    // 3. 依次出栈打印出来（逆序）: n ... 4 3 2 1
    int k;
    while(!stack_is_empty(s))
    {
        pop(s, &k);
        printf("%d ", k);
    }
    printf("\n");

    return 0;
}

