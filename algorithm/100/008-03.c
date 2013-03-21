#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *next;
};

static struct node *build_list(int size)
{
	struct node *head, *n;
	int i;

	head = NULL;
	for (i = 0; i < size; i++) {
		n = malloc(sizeof(struct node));	
		n->next = head;
		n->data = i;
		head = n;
	}
	return head;
}

static void free_list(struct node *head)
{
	struct node *p;

	while (head) {
		p = head;
		head = head->next;
		free(p);
	}
}

static void dump_list(struct node *head)
{
	while (head) {
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

static struct node *recursive_reverse_list(struct node *head, struct node **new_head)
{
	struct node *tail;

	if (!head->next) {
		*new_head = head;
	} else {
		tail = recursive_reverse_list(head->next, new_head);
		tail->next = head;
	}

	head->next = NULL;
	tail = head;

	return tail;	
}

static struct node *recursive_reverse_list2(struct node *head)
{
	struct node *new_head;

	if (!head || !head->next)
		return head;

	new_head = recursive_reverse_list2(head->next);
	head->next->next = head;
	head->next = NULL;

	return new_head;
}

static struct node *reverse_list(struct node *head)
{
	struct node *new_head = NULL;
	struct node *n;

	while (head) {
		n = head;
		head = head->next;

		n->next = new_head;
		new_head = n;
	}

	return new_head;
}

int main()
{
	struct node *head, *new_head;

	head = build_list(10);
	dump_list(head);

	recursive_reverse_list(head, &new_head);
	dump_list(new_head);

	new_head = reverse_list(new_head);
	dump_list(new_head);

	new_head = recursive_reverse_list2(new_head);
	dump_list(new_head);

	free_list(new_head);

	return 0;
}
