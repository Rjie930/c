#include <stdio.h>
int main()
{
    int sum = 0;
    int a[7] = {-2, 5, -1, 6, -4, -8, 6};
    int max = 0;
    int len = sizeof(a) / sizeof(int);
    for (int i = len; i > 0; i--)         // sum
        for (int k = 0; k < len - i; k++) // post
        {
            sum = 0;
            for (int j = k; j < k + i; j++) // add
            {
                sum += a[j];
                if (sum > max)
                    max = sum;
            }
        }
    printf("max:%d\n", max);
    return 0;
}