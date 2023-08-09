#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void main(int argc, char **argv)
{
    if ((argc != 2) && (argc != 3))
    {
        printf("参数有误!\n");
        return -1;
    }
    int num;
    int i = 0;
    int sum = 0;
    char a[100][15];
    char(*p)[15] = a;

    if (argc == 3)
        num = atoi(argv[2]);
    else
        num = 1;
    FILE *fp = fopen(argv[1], "r");
    if (fp < 0)
    {
        printf("can't open %s\n", argv[2]);
        return -1;
    }
    bzero(a, 1500);
    while (fgets(p + sum++, 15, fp));
    sum-=1;

    while (i < sum-1 )
        *strchr(p + i++, '\n') = '\0';

    int times = 0;
    srand(time(0));
    while (times < 15)
    {
        for (int i = 0; i < num; i++)
        {
            char(*tmp)[15];
            do
            {
                tmp = p + (rand() % sum);
            } while (*tmp[0] == '\0');

            printf("%s ", tmp);
            if (times == 14)
                *tmp[0] = '\0';
        }
        for (int i = 0; i < 2 * num; i++)
            putchar(' ');
        putchar('\r');
        fflush(stdout);
        usleep(times++ * 50000);
    }
    putchar('\n');
}