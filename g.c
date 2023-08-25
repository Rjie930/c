#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>

int global = 100;

void *isPrime(void *arg)
{
    while(1)
    {
        // 一段朴素的代码
        if(global%2 == 0)
        if(1)
        if(1)
        if(1)
            printf("%d是偶数\n", global);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, isPrime, NULL);

    // 一条人畜无害的赋值语句
    while(1)
        global = rand() % 5000;
}