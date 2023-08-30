#include <stdio.h>

int main(void)
{
	int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
	int (*b[2])[3];

	b[0] = &a[0];
	b[1] = &a[1];

	int i, j, sum=0;
	for(i=0; i<2; ++i)
	{
		for(j=0; j<3; ++j)
		{
			sum += (*b[i])[j];
		}
	}

	printf("sum: %d\n", sum);
	return 0;
}