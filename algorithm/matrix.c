#include <stdio.h>

/*
 * An example: (5,2)*(2,10)*(10,3)*(3,6)*(6,8)*(8,11)
 */
#define NUM 6 
int p[NUM+1] = {5, 2, 10, 3, 6, 8, 11};
int m[NUM][NUM];
int s[NUM][NUM];

static void min(int i, int j)
{
	int k;
	int tmp;

	for (k = i; k < j; k++) {
		tmp = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
		if (!m[i][j] || tmp < m[i][j])
			m[i][j] = tmp;
	}
}

static void matrix()
{
	int l, i, j;

	for (l = 1; l < NUM; l++) {
		for (i = 0; i < NUM - l; i++) {
			j = i + l;
			min(i, j); 
		}
	}
}

int main()
{
	matrix();

	printf("m[0][%d]=%d\n", NUM-1, m[0][NUM-1]);
	return 0;
}
