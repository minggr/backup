#include <stdio.h>

#define NUM 6
int e[2] = {2, 4};
int x[2] = {3, 2};
int a[2][NUM] = {{7, 9, 3, 4, 8, 4}, {8, 5, 6, 4, 5, 7}};
int t[2][NUM-1] = {{2, 3, 1, 3, 4}, {2, 1, 2, 2, 1}};
int T[2][NUM];
int l[2][NUM];

void assemble()
{
	int j;
	int tmp1, tmp2;

	T[0][0] = e[0] + a[0][0];
	T[1][0] = e[1] + a[1][0];

	for (j = 1; j < NUM; j++) {
		//T[0][j]
		tmp1 = T[0][j-1] + a[0][j]; 
		tmp2 = T[1][j-1] + t[1][j-1] + a[0][j]; 
		T[0][j] = tmp1 < tmp2 ? tmp1 : tmp2;
		l[0][j] = tmp1 < tmp2 ? 0 : 1;

		//T[1][j]
		tmp1 = T[1][j-1] + a[1][j]; 
		tmp2 = T[0][j-1] + t[0][j-1] + a[1][j]; 
		T[1][j] = tmp1 < tmp2 ? tmp1 : tmp2;
		l[1][j] = tmp1 < tmp2 ? 1 : 0;
	}
}

void debug()
{
	int i, j;

	for (i = 0; i < 2; i++) {
		for (j = 1; j < NUM; j++) {
			printf("l[%d][%d]=%d\n", i, j, l[i][j]+1);
		}
	}
}

int main()
{
	int total;
	int tmp1, tmp2;
	int j, last;

	assemble();
	tmp1 = T[0][NUM-1] + x[0];
	tmp2 = T[1][NUM-1] + x[1];
	total = tmp1 < tmp2 ? tmp1 : tmp2;
	printf("total = %d\n", total);

	last = tmp1 < tmp2 ? 0 : 1;
	printf("line %d, station %d\n", last+1, NUM);
	for (j = NUM-1; j > 0; j--) {
		printf("line %d, station %d\n", l[last][j]+1, j);
		last = l[last][j];
	}

	//debug();

	return 0;
}
