#include <stdio.h>

#define NUM 4
int a[NUM] = {4, 5, 9, 4};
int m[NUM][NUM+1];

void init()
{
	int i;

	for (i = 0; i < NUM; i++)
		m[i][1] = a[i];
}

void fnit()
{
	int i, j;

	for (i = 0; i < NUM; i++)
		for (j = 1; j <= NUM; j++)
			m[i][0] += m[i][j];
}

void merge()
{
	int l;
	int i;
	int last;
	int tmp;

	for (l = 2; l <= NUM; l++) {
		for (i = 0; i < NUM; i++) {
			last = (i+l-1)%NUM;

			tmp = m[i][l-1] + a[last];
			if (!m[i][l] || tmp < m[i][l])
				m[i][l] = tmp;
		}
	}
}

void debug()
{
	int i, j;

	for (i = 0; i < NUM; i++)
		for (j = 0; j <= NUM; j++)
			printf("m[%d][%d]=%d\n", i, j, m[i][j]);
}

int main()
{
	int i;

	init();
	merge();
	fnit();
	debug();

	printf("\n");
	for (i = 0; i < NUM; i++)
		printf("m[%d][0]=%d\n", i, m[i][0]);

	return 0;
}
