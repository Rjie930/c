#include <stdio.h>
#include <stdlib.h> // enable the function rand()

// 将数组source各个元素的值，赋给数组target
void assign(int row, int col,
            double target[row][col], double source[row][col])
{
    int i, j;
    for(i=row; i>0; i--)
    {
        for(j=col; j>0; j--)
        {
            target[i-1][j-1] = source[i-1][j-1];
        }
    }
}

void show(int row, int col,
           double target[row][col], double source[row][col])
{
    int i, j;
    printf("源数据:\n");

    for(i=row; i>0; i--)
    {
        for(j=col; j>0; j--)
        {
            printf("%f\t", source[i-1][j-1]);
        }
        printf("\n");
    }

    printf("目标数据:\n");
    for(i=row; i>0; i--)
    {
        for(j=col; j>0; j--)
        {
            printf("%f\t", target[i-1][j-1]);
        }
        printf("\n");
    }
}

int main(void)
{
    int i, j;
    int row, col;
    
    printf("请输入二维数组的行数和列数：\n");
    scanf("%d%d", &row, &col);

    double source[row][col];

    for(i=0; i<row; i++)
    {
        for(j=0; j<col; j++)
        {
            // 往源数组中写入随机数据
            source[i][j] = (double)rand() / (double)rand();
        }
    }

    double target[row][col];

    assign(row, col, target, source);
    show(row, col, target, source);

    return 0;
}