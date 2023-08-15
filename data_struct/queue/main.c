/*
 * @Author: vincent
 * @Date: 2023-08-15 11:20:32
 * @LastEditors: veincent
 * @LastEditTime: 2023-08-15 11:56:53
 * @Description: 
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// #define DATATYPE ....
#include "queue.h"

int main(int argc, char const *argv[])
{
    // 1. 搞一个空队列
    linkqueue q = init_queue();

    // 2. 让一些自然数排队: 1 2 3 4 5 
    int n;
    scanf("%d", &n);
    for(int i=1; i<=n; i++)
        en_queue(q, i);

    // 3. 出队观察其顺序： 1 2 3 4 5
    int k;
    while(!queue_is_empty(q))
    {
        out_queue(q, &k);
        printf("%d ", k);
    }

    // 4. 销毁队列
    linkqueue_destroy(q, NULL);
    q = NULL;

    return 0;
}
