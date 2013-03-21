#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void heapsort(int *a, int n)
{
	int i;
	int max;
	int tmp;

	build_max_heap(a, n);

	for (i = n - 1; i > 0; i--) {
		max = a[0];
		a[0] = a[i];
		a[i] = max;
		max_heapify(a, i, 0);
	}
}

#define NUM (10000*100)
int main()
{
	int *a = malloc(NUM);

	if (!a) {
		printf("malloc fail\n");
		return -1;
	}
	
	heapsort(a, NUM);
	return 0;
}
