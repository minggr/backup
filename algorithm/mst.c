#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disjoint_set.h"
#include "graph.h"

#define NUM_VERTEX 8
#define NUM_EDGE 17

static int vertex_data[NUM_VERTEX] = {1, 2, 3, 4, 5, 6, 7, 8};
static int edge_data[NUM_EDGE][3] = {
	{1, 2, 6}, {1, 3, 4}, {1, 8, 2},
	{3, 1, 4}, {3, 2, 1}, {3, 8, 2}, {3, 4, 8}, {3, 7, 4},
	{4, 3, 8}, {4, 7, 5},
	{7, 8, 9}, {7, 3, 4}, {7, 4, 5}, {7, 5, 4}, {7, 6, 1},
	{5, 7, 4}, {5, 6, 2}
};

//insert sort edge by weight
static void mst_kruskal_sort_edge(struct graph *graph)
{
	struct edge *e;
	int w;
	int i, j;

	for (i = 1; i < graph->num_edge; i++) {
		e = graph->pe[i];
		w = e->w;

		for (j = i-1; j >= 0; j--) {
			if (graph->pe[j]->w > w)
				graph->pe[j+1] = graph->pe[j];
			else
				break;
		}
		graph->pe[j+1] = e;
	}
}

static void dump_edge(struct edge **pe, int size)
{
	struct edge *e;
	int i = 0;

	for (i = 0; i < size; i++) {
		e = pe[i];
		if (!e)
			break;
		printf("(%d, %d: %d)\n", e->u->data, e->v->data, e->w);
	}
}

static void mst_kruskal_make_set(struct graph *graph)
{
	int i;

	for (i = 0; i < graph->num_vertex; i++) {
		make_set(&graph->v[i].d_node);
	}
}

static struct vertex *mst_kruskal_find_set(struct vertex *v)
{
	struct disjoint_node *n;

	n = find_set(&v->d_node);
	return disjoint_node_to_vertex(n);
}

static void mst_kruskal_union_set(struct vertex *u, struct vertex *v)
{
	union_set(&u->d_node, &v->d_node);
}

static int in_set(struct edge **pe, int size, struct edge *e)
{
	int i;

	for (i = 0; i < size; i++)
		if (pe[i] == e)
			return 1;
	return 0;
}

static struct edge **mst_kruskal(struct graph *graph)
{
	struct edge **pe, *e;
	struct vertex *set1, *set2;
	int i, j = 0;

	mst_kruskal_make_set(graph);
	mst_kruskal_sort_edge(graph);
	pe = malloc(graph->num_edge * sizeof(struct edge *));
	memset(pe, 0, graph->num_edge * sizeof(struct edge *));

	for (i = 0; i < graph->num_edge; i++) {
		e = graph->pe[i];
		if (in_set(pe, graph->num_edge, e))
			continue;
		set1 = mst_kruskal_find_set(e->u);
		set2 = mst_kruskal_find_set(e->v);
		if (set1 != set2) {
			pe[j++] = e;	
			mst_kruskal_union_set(e->u, e->v);
		}
	}

	return pe;
}

int main()
{
	struct graph *graph;
	struct edge **pe;

	graph = init_graph(vertex_data, NUM_VERTEX, edge_data, NUM_EDGE);
	pe = mst_kruskal(graph);
	dump_edge(pe, graph->num_edge);
	
	free(pe);
	free(graph);

	return 0;
}
