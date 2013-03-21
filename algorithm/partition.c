#include <stdlib.h>

int partition(int data[], int left, int right, int random)
{
	int i, k;
	int pivot;
	int tmp;

	if (random) {
		int p;

		srand((unsigned int)time(NULL));
		p = rand() % (right - left + 1);
		p += left;
		tmp = data[p];
		data[p] = data[right];
		data[right] = tmp;
	}

	pivot = data[right];

	k = left;
	for (i = left; i < right; i++) {
		if (data[i] < pivot) {
			//exchange data[i] and data[k]
			tmp = data[i];
			data[i] = data[k];
			data[k] = tmp;
			k++;
		}
	}

	tmp = data[k];
	data[k] = data[right];
	data[right] = tmp;

	return k;
}
