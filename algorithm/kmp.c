#include <stdio.h>

#define SIZE 10
static int prefix_function[SIZE];

static void compute_prefix_function(char pattern[], int m)
{
	int k = 0;
	int q;

	prefix_function[0] = 0;	
	
	for (q = 1; q < m; q++) {
		while (k > 0 && pattern[k] != pattern[q])
			k = prefix_function[k];
		if (pattern[k] == pattern[q])
			k = k + 1;
		prefix_function[q] = k;
	}
}

static void dump_data(int data[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		printf("%d ", data[i]);
	printf("\n");
}

int main()
{
	char pattern[SIZE+1] = "ababababca";	

	compute_prefix_function(pattern, SIZE);
	dump_data(prefix_function, SIZE);

	return 0;
}
