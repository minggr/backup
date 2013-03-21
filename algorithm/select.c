#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "partition.h"
#include "util.h"

/* n number, find k-th big number */
int my_select(int data[], int left, int right, int k)
{
	int p;

	p = partition(data, left, right, 1);
	if (p == k)
		return data[p];
	else if (p < k)
		return my_select(data, p+1, right, k);
	else
		return my_select(data, left, p-1, k);
}

#define NUM (10000*100)
int main()
{
	int *data = create_rand_nums(NUM);

	assert(data);

	printf("%d\n", my_select(data, 0, NUM-1, 300));

	free(data);

	return 0;
}
