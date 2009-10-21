#include <stdio.h>

struct node {
	int value;
	struct node *child, *parent, *next;
};

typedef void (*callback) (struct node *);

void descending(struct node *node)
{
	printf("descending: %d\n", node->value);
}
void ascending(struct node *node)
{
	printf("ascending: 0x%d\n", node->value);
}

void walk(struct node *root, callback descending, callback ascending)
{
	struct node *node = root;
	struct node *next;
	int visited = 0;

	while (node) {
		if (visited) {
			ascending (node);
		} else {
			descending (node);

			/* visit children */
			next = node->child;
			if (next) {
				node = next;
				continue;
			}
		}

		if (node == root)
			break;

		/* No more children, re-visit this node */
		if (!visited) {
			visited = 1;
			continue;
		}

		/* No more children, visit peers */
		if (node->next) {
			node = node->next;
			visited = 0;
		} else {
			/* No peers, re-visit parent */
			node = node->parent;
			visited = 1;
		}
	}
}

int main()
{
	struct node *root;
	struct node n1, n2, n3, n4, n5, n6;

	n1.value = 1; n1.next = NULL; n1.parent = NULL; n1.child = &n2;
	n2.value = 2; n2.next = &n3;  n2.parent = &n1;  n2.child = NULL;
	n3.value = 3; n3.next = &n6;  n3.parent = &n1;  n3.child = &n4;
	n4.value = 4; n4.next = &n5;  n4.parent = &n3;  n4.child = NULL;
	n5.value = 5; n5.next = NULL; n5.parent = &n3;  n5.child = NULL;
	n6.value = 6; n6.next = NULL; n6.parent = &n1;  n6.child = NULL;

	root = &n1;

	walk(root, descending, ascending);
}
