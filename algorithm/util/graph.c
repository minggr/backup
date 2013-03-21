#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

static struct vertex *find_vertex(struct graph *graph, int data)
{
	int i;

	for (i = 0; i < graph->num_vertex; i++) {
		if (graph->v[i].data == data)
			return &graph->v[i];
	}

	return NULL;
}

static void insert_adj_table(struct adj_table *adj, struct vertex *u)
{
	adj->v[adj->num_edge] = u;
	adj->num_edge++;
}

struct graph *init_graph(int *vertex_data, int num_vertex, int edge_data[][3], int num_edge)
{
	struct graph *graph;
	struct adj_table *adj;
	int i;

	graph = malloc(sizeof(struct graph));
	graph->v = malloc(num_vertex * sizeof(struct vertex));
	graph->e = malloc(num_edge * sizeof(struct edge));
	graph->pe = malloc(num_edge * sizeof(struct edge *));
	graph->num_vertex = num_vertex;
	graph->num_edge = num_edge;

	for (i = 0; i < num_vertex; i++) {
		graph->v[i].data = vertex_data[i];
		graph->v[i].color = WHITE;
		adj = malloc(sizeof(struct adj_table));
		graph->v[i].adj = adj;
		graph->v[i].adj->max_edge = num_edge;
		graph->v[i].adj->num_edge = 0;
		graph->v[i].adj->v = malloc(num_edge * sizeof(struct vertex *));
	}

	for (i = 0; i < num_edge; i++) {
		graph->e[i].u = find_vertex(graph, edge_data[i][0]);
		graph->e[i].v = find_vertex(graph, edge_data[i][1]);
		graph->e[i].w = edge_data[i][2];
		graph->pe[i] = &graph->e[i];
		insert_adj_table(graph->e[i].u->adj, graph->e[i].v);
	}

	return graph;
}

void free_graph(struct graph *graph)
{
	int i;

	for (i = 0; i < graph->num_vertex; i++) {
		free(graph->v[i].adj->v);
		free(graph->v[i].adj);
	}

	free(graph->v);
	free(graph->e);
	free(graph);
}

void dump_graph(struct graph *graph)
{
	int i, j;

	printf("Dump graph: %d vertexes, %d edges\n", graph->num_vertex, graph->num_edge);

	printf("Vertexes:\n");
	for (i = 0; i < graph->num_vertex; i++) {
		printf("  %d: adj_table: ", graph->v[i].data);
		for (j = 0; j < graph->v[i].adj->num_edge; j++)
			printf("%d ", graph->v[i].adj->v[j]->data);
		printf("\n");
	}

	printf("Edges: \n");
	for (i = 0; i < graph->num_edge; i++) {
		printf("  (%d, %d: %d)\n", graph->e[i].u->data, graph->e[i].v->data, graph->e[i].w);
	}
}
