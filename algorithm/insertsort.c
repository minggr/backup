#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void insertsort(int *a, int n)
{
	int i, j;
	int key;

	for (i = 1; i < n; i++) {
		key = a[i];
		
		j = i - 1;
		while (j >= 0 && a[j] > key) {
			a[j+1] = a[j];	
			j--;
		}

		if (j != i - 1)
			a[j+1] = key;
	}
}

#define NUM (10000*1000)

int main()
{
	int *a;
	int i;

	a = malloc(NUM * sizeof(int));
	assert(a != NULL);

	for (i = 0; i < NUM; i++)
		a[i] = NUM - i;
	insertsort(a, NUM);

	return 0;
}
