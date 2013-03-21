#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

static int parent(int i)
{
	return (i+1)/2 - 1;
}

static int left(int i)
{
	return (i+1)*2 - 1;
}

static int right(int i)
{
	return (i+1)*2;
}

static void max_heapify(struct heap *heap, int i)
{
	int largest;
	int l = left(i);
	int r = right(i);

	largest = i;
	if (l < heap->size && heap->data[l] > heap->data[largest])
		largest = l;
	if (r < heap->size && heap->data[r] > heap->data[largest])
		largest = r;

	if (largest != i) {
		int tmp = heap->data[i];
		heap->data[i] = heap->data[largest];
		heap->data[largest] = tmp;
		max_heapify(heap, largest);
	}
}

struct heap *build_max_heap(int *data, int size)
{
	struct heap *heap;
	int i = size / 2;

	heap = malloc(sizeof(struct heap));
	heap->data = data;
	heap->size = heap->max_size = size;

	while (i--) {
		max_heapify(heap, i);
	}

	return heap;
}

static void min_heapify(struct heap *heap, int i)
{
	int smallest;
	int l = left(i);
	int r = right(i);

	smallest = i;
	if (l < heap->size && heap->data[l] < heap->data[smallest])
		smallest = l;
	if (r < heap->size && heap->data[r] < heap->data[smallest])
		smallest = r;

	if (smallest != i) {
		int tmp = heap->data[i];
		heap->data[i] = heap->data[smallest];
		heap->data[smallest] = tmp;
		min_heapify(heap, smallest);
	}
}

struct heap *build_min_heap(int *data, int size, int max_size)
{
	struct heap *heap;
	int i = size / 2;

	heap = malloc(sizeof(struct heap));
	heap->data = data;
	heap->size = size;
	heap->max_size = max_size;

	while (i--) {
		min_heapify(heap, i);
	}

	return heap;
}

int min_heap_extract(struct heap *heap)
{
	int min = heap->data[0];	

	heap->data[0] = heap->data[heap->size-1];
	heap->size--;

	min_heapify(heap, 0);
	return min;
}

void min_heap_insert(struct heap *heap, int data)
{
	int i;

	if (heap->size == heap->max_size) {
		printf("heap full\n");
		return;
	}

	i = heap->size;
	heap->data[heap->size] = data;
	heap->size++;

	while (i >= 0) {
		int tmp;

		if (heap->data[i] < heap->data[parent(i)]) {
			tmp = heap->data[i];
			heap->data[i] = heap->data[parent(i)];
			heap->data[parent(i)] = tmp;
		}

		i = parent(i);
	}
}

#if 0
static void dump_heap(struct heap *heap)
{
	int i;

	for (i = 0; i < heap->size; i++)
		printf("%d ", heap->data[i]);
	printf("\n");
}

int main()
{
	int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int size = 10;
	//int *data = malloc(sizeof(int)*size);
	struct heap *h;
	int new_data = 30;

	h = build_max_heap(data, size);
	dump_heap(h);
	free(h);

	h = build_min_heap(data, size/2, size);
	dump_heap(h);
	printf("extract min=%d\n", min_heap_extract(h));
	dump_heap(h);
	printf("insert %d\n", new_data);
	min_heap_insert(h, new_data);
	dump_heap(h);
	free(h);

	//free(data);

	return 0;
}
#endif
