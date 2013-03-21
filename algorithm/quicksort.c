#include <stdio.h>
#include <stdlib.h>
#include "partition.h"

void quicksort(int data[], int left, int right, int random)
{
	int i;

	if (left < right) {
		i = partition(data, left, right, random);
		quicksort(data, left, i-1, random);
		quicksort(data, i+1, right, random);
	}
}

static void dump(int data[], int n)
{
	int i;

	for (i = 0; i < n; i++)
		printf("%d ", data[i]);
	printf("\n");
}

#define NUM (10000*1000)

int main()
{
	//int data[10] = {10, 9, 8, 7, 1, 2, 3, 4, 5, 6};
	int *data = malloc(NUM * sizeof(int));
	int i;
	int t1, t2;

	printf("RAND_MAX=%d\n", RAND_MAX);

	if (!data) {
		printf("malloc fail\n");
		return -1;
	}

	srand((unsigned int)time(NULL));
	for (i = 0; i < NUM; i++)
		data[i] = rand()%NUM;

	t1 = time(NULL);
	quicksort(data, 0, NUM-1, 1);
	t2 = time(NULL);
	printf("%d seconds\n", t2 - t1);
	//quicksort(data, 0, NUM-1, 0);
	//printf("%d\n", time(NULL));
	
	free(data);

	return 0;
}
