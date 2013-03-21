#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node *listsort(struct node *head)
{
	struct node *sorted_head = NULL;
	struct node **n;
	struct node *node;

	while (head) {
		n = &sorted_head;
		node = head;
		head = head->next;
		
		while (1) {
			if (!(*n) || node->data < (*n)->data) {
				node->next = *n;
				*n = node;
				break;
			} else
				n = &((*n)->next);
		}
	}

	return sorted_head;
}

static void dump(struct node *head)
{
	while (head) {
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

#define NUM (10000*1000)

int main()
{
	struct node *head;
	int i;

	head = malloc(sizeof(struct node) * NUM);
	assert(head);
	for (i = 0; i < NUM - 1; i++) {
		head[i].data = NUM - i;
		head[i].next = &head[i+1];
	}
	head[i].data = NUM - i;
	head[i].next = NULL;

	//dump(head);
	head = listsort(head);
	//dump(head);

	return 0;
}
