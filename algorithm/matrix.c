#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM 6 
int p[NUM+1] = {5, 2, 10, 3, 6, 8, 11};
int m[NUM][NUM];
int s[NUM][NUM];

static void min(int i, int j)
{
	int k;
	int tmp;

	if (i == j) {
		m[i][j] = 0;
		return;
	}

	for (k = i; k < j; k++) {
		if (m[i][k] < 0 || m[k+1][j] < 0) {
			printf("error\n");
		}
		tmp = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
		if (!m[i][j] || tmp < m[i][j])
			m[i][j] = tmp;
	}
}

static void matrix()
{
	int l, i, j;

	for (l = 1; l < NUM; l++) {
		printf("\nl=%d\n", l);
		for (i = 0; i < NUM - l; i++) {
			j = i + l;
			printf("i=%d, j=%d\n", i, j);
			min(i, j); 
		}
	}
}

static void init()
{
	int i, j;

	for (i = 0; i < NUM; i++)
		for (j = i; j < NUM; j++)
			m[i][j] = -1;
}

int main()
{
	int i, j;

	//init();
	matrix();

	for (i = 0; i < NUM; i++)
		for (j = i; j < NUM; j++)
			printf("m[%d][%d]=%d\n", i, j, m[i][j]);
	printf("\n");
	printf("m[0][%d]=%d\n", NUM-1, m[0][NUM-1]);
	return 0;
}
