#include <stdio.h>

#define SIZE 8
static int mydata[SIZE] = {1, -2, 3, 10, -4, 7, 2, -5};
static int m[SIZE][SIZE];

static int sum(int data[], int i, int j)
{
	int s = 0;
	int k;

	for (k = i; k <= j; k++)
		s += data[k];
	return s;
}

static int max(int data[], int i, int j)
{
	if (i == j) {
		m[i][j] = data[i];
	} else {
		int m1;
		int s;

		if (data[i] >= data[j])
			m1 = max(data, i, j-1);
		else
			m1 = max(data, i+1, j);
		s = sum(data, i, j);
		m[i][j] = m1 > s ? m1 : s;
	}

	return m[i][j];
}

static int greedy_max(int data[], int n)
{
	int m;
	int s;
	int i;

	m = s = data[0];
	for (i = 1; i < n; i++) {
		s += data[i];
		if (s < 0)
			s = 0;
		if (s > m)
			m = s;
	}

	return m;
}

int main()
{
	printf("%d\n", max(mydata, 0, SIZE-1));

	printf("%d\n", greedy_max(mydata, SIZE));

	return 0;
}
