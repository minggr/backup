#include <stdio.h>

#define SIZE 8
int mydata[SIZE] = {7, 5, 4, 8, 3, 1, 2, 6};
int least = 4;

int partition(int data[], int i, int j)
{
	int tmp = data[j];
	int k;
	int p;

	if (i == j)
		return i;

	p = i;
	for (k = i+1; k < j; k++) {
		if (data[k] < tmp) {
			int t = data[k];		
			data[k] = data[p];
			data[p] = t;
			p++;
		} 
	}

	if (p == least)
		return p;
	else {
		if (p < least)
			return partition(data, p, j);
		else
			return partition(data, i, p);
	}
}

void print(int p)
{
	int i;

	for (i = 0; i < p; i++)
		printf("%d ", mydata[i]);
	printf("\n");
}

int main()
{
	partition(mydata, 0, SIZE-1);
	print(4);

	return 0;
}
