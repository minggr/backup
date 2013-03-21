#ifndef _DISJOINT_SET_H_
#define _DISJOINT_SET_H_

struct disjoint_node {
	struct disjoint_node *parent;
	int rank;
};

void make_set(struct disjoint_node *node);

struct disjoint_node *find_set(struct disjoint_node *node);

void union_set(struct disjoint_node *n1, struct disjoint_node *n2);

#endif
