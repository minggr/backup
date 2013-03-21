void max_heapify(int *a, int n, int i)
{
	int left, right;
	int largest;
	int tmp;

	left = 2 * i + 1;
	right = 2 * i + 2;

	largest = i;
	if (left < n && a[left] > a[largest])
		largest = left;
	if (right < n && a[right] > a[largest])
		largest = right;

	if (largest != i) {
		tmp = a[i];
		a[i] = a[largest];
		a[largest] = tmp;
		max_heapify(a, n, largest);
	}
}

void build_max_heap(int *a, int n)
{
	int i;

	for (i = n / 2 - 1; i >= 0; i--)
		max_heapify(a, n, i);
}
