#include <stdlib.h>

int *create_rand_nums(int n)
{
	int *data = malloc(n * sizeof(int));	
	int i;

	if (!data)
		return NULL;

	srand((unsigned int)time(NULL));
	for (i = 0; i < n; i++)
		data[i] = rand() % n;

	return data;
}
