//http://en.wikipedia.org/wiki/Permutation#Algorithms_to_generate_permutations

#include <stdio.h>
#define NUM 11
int data[NUM];

void output()
{
	int i;

	for (i = 0; i < NUM; i++)
		printf("%d ", data[i]);
	printf("\n");
}

void init()
{
	int i;

	for (i = 0; i < NUM; i++)
		data[i] = i + 1;
}

int next_permutation()
{
	int k, l;
	int i;
	int tmp;

	i = NUM - 1;
	while (i > 0 && data[i] < data[i-1])
		i--;
	if (i == 0)
		return 0;
	k = i - 1;

	i = NUM - 1;
	while (i >= 0 && data[i] < data[k])
		i--;
	l = i;

	tmp = data[k];
	data[k] = data[l];
	data[l] = tmp;

	k = k + 1;
	l = NUM - 1;
	while (k < l) {
		tmp = data[k];
		data[k] = data[l];
		data[l] = tmp;

		k++;	
		l--;
	}

	return 1;
}

int main()
{
	int i = 0;

	init();

	do {
		output();
		i++;
	} while (next_permutation());

	printf("Total: %d\n", i);

	return 0;
}
