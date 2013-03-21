#include "disjoint_set.h"

void make_set(struct disjoint_node *node)
{
	node->parent = node;
	node->rank = 0;
}

struct disjoint_node *find_set(struct disjoint_node *node)
{
	if (node->parent != node)
		node->parent = find_set(node->parent);
	return node->parent;
}

void union_set(struct disjoint_node *n1, struct disjoint_node *n2)
{
	struct disjoint_node *set1 = find_set(n1);
	struct disjoint_node *set2 = find_set(n2);

	if (set1 == set2)
		return;

	if (set1->rank < set2->rank)
		set1->parent = set2;
	else {
		set2->parent = set1;
		if (set1->rank == set2->rank)
			set1->rank++;
	}
}
