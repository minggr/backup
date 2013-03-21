#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "disjoint_set.h"

enum {
	WHITE,
	GRAY,
	BLACK
};

struct vertex;
struct adj_table {
	struct vertex **v;
	int num_edge;
	int max_edge;
};

struct vertex {
	int data;
	int color;
	struct adj_table *adj;
	struct disjoint_node d_node;
};

struct edge {
	struct vertex *u, *v;
	int w; //weight
};

struct graph {
	struct vertex *v; //vertex array
	int num_vertex;
	struct edge *e; //edge array
	struct edge **pe; //edge array
	int num_edge;
};

struct graph *init_graph(int *vertex_data, int num_vertex, int edge_data[][3], int num_edge);
void free_graph(struct graph *graph);
void dump_graph(struct graph *graph);

#define disjoint_node_to_vertex(node) (struct vertex *)((char*)node - (char*)&((struct vertex *)NULL)->d_node)

#endif
