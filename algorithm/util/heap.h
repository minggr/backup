#ifndef _HEAP_H_
#define _HEAP_H_

struct heap {
	int *data; //array
	int size;
	int max_size;
};

struct heap *build_max_heap(int *data, int size);
void max_head_insert(struct heap *heap, int data);
void max_head_extract(struct heap *heap);

#endif
