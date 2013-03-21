#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int data[10];

int main(int argc, char *argv[])
{
	unsigned int seed;
	int nloops = 100*10000;
	int i;

	seed = (unsigned int)time(NULL);
	srand(seed);

	for (i = 0; i < nloops; i++)
		data[rand()%10]++;

	for (i = 0; i < 10; i++)
		printf("%f\n", (float)data[i]/(float)nloops);

	return 0;
}

