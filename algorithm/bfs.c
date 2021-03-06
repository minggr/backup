#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTEX 8
#define NUM_EDGE 17
#define MAX_EDGE NUM_EDGE

enum {
	WHITE,
	GRAY,
	BLACK
};

struct vertex {
	int data;
	int color;
	struct vertex *edge[MAX_EDGE];
	int num_edge;
	int distance;
	struct vertex *parent;
	struct vertex *next; //queue
};

struct edge {
	struct vertex v[2];
};

struct adj_table {
	struct vertex *vs;
	int num_vertex;
	int num_edge;
};

struct queue {
	struct vertex *head;
	struct vertex *tail;
};

void init_queue(struct queue *q)
{
	q->head = q->tail = NULL;
}

void enqueue(struct queue *q, struct vertex *v)
{
	//printf("enqueue: %d\n", v->data);
	v->next = NULL;	

	if (!q->tail)
		q->head = q->tail = v;
	else {
		q->tail->next = v;
		q->tail = v;
	}
}

struct vertex *dequeue(struct queue *q)
{
	struct vertex *v;

	if (!q->head)
		return NULL;

	v = q->head;
	q->head = v->next;
	if (!q->head)
		q->tail = NULL;

	//printf("dequeue: %d\n", v->data);

	return v;
}

struct vertex *search_adj_table(struct adj_table *adj, int data)
{
	int i;	

	for (i = 0; i < adj->num_vertex; i++)
		if (adj->vs[i].data == data)
			return &adj->vs[i];
	return NULL;
}

struct adj_table *init_adj_table(int *vertex_data, int num_vertex, int edge_data[][2], int num_edge)
{
	struct adj_table *adj_table;
	struct vertex *v, *v_edge;
	int i;

	adj_table = malloc(sizeof(struct adj_table));
	adj_table->vs = malloc(num_vertex * sizeof(struct vertex));
	adj_table->num_vertex = num_vertex;
	adj_table->num_edge = 0;
	for (i = 0; i < num_vertex; i++) {
		adj_table->vs[i].data = vertex_data[i];
		adj_table->vs[i].color = WHITE;
		adj_table->vs[i].num_edge = 0;
		adj_table->vs[i].distance = 0;
		adj_table->vs[i].parent = NULL;
	}

	for (i = 0; i < num_edge; i++) {
		v = search_adj_table(adj_table, edge_data[i][0]);
		v_edge = search_adj_table(adj_table, edge_data[i][1]);
		v->edge[v->num_edge] = v_edge;
		v->num_edge++;
		adj_table->num_edge++;
	}

	return adj_table;
}

void print_vertex_edge(struct vertex *v)
{
	int i;

	printf("%d: ", v->data);
	
	for (i = 0; i < v->num_edge; i++)
		printf("%d ", v->edge[i]->data);
	printf("\n");
}

void print_adj_table(struct adj_table *adj)
{
	int i;

	printf("num_vertex=%d\n", adj->num_vertex);
	printf("num_edge=%d\n", adj->num_edge);

	for (i = 0; i < adj->num_vertex; i++)
		print_vertex_edge(adj->vs + i);
}

void dump_queue(struct queue *q)
{
	struct vertex *v = q->head;

	printf("dump queue: ");
	while (v) {
		printf("%d ", v->data);
		v = v->next;
	}
	printf("\n");
}

void bfs(struct adj_table *adj, struct queue *q)
{
	struct vertex *v;
	struct vertex *edge;
	int i;

	adj->vs->color = GRAY;
	adj->vs->parent = NULL;
	adj->vs->distance = 0;
	enqueue(q, adj->vs);

	v = dequeue(q);
	while (v) {
		for (i = 0; i < v->num_edge; i++) {
			if (v->edge[i]->color == WHITE) {
				v->edge[i]->color = GRAY;
				v->edge[i]->parent = v;
				v->edge[i]->distance = v->distance + 1;
				enqueue(q, v->edge[i]);
			}
		}
		printf("%d ", v->data);
		v = dequeue(q);
	}
	printf("\n");
}

void print_path(struct vertex *v1, struct vertex *v2)
{
	if (v1 == v2) {
		printf("%d ", v1->data);
		return;
	}

	if (!v2->parent) {
		printf(" no path");
		return;
	} else {
		print_path(v1, v2->parent);
		printf("%d ", v2->data);
	}
}

int vertex_data[NUM_VERTEX] = {1, 2, 3, 4, 5, 6, 7, 8};
int edge_data[NUM_EDGE][2] = {{1, 2}, {1, 3}, {1, 8}, {3, 1}, {3, 2}, {3, 8}, {3, 4}, {3, 7}, {4, 3}, {4, 7}, {7, 8}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {5, 7}, {5, 6}};

int main()
{
	struct adj_table *adj;
	struct queue myq;
	int i;

	adj = init_adj_table(vertex_data, NUM_VERTEX, edge_data, NUM_EDGE);
	print_adj_table(adj);
	printf("\n");

	init_queue(&myq);
	bfs(adj, &myq);

	for (i = 0; i < NUM_VERTEX; i++) {
		struct vertex *v1 = search_adj_table(adj, 7);
		struct vertex *v2 = search_adj_table(adj, vertex_data[i]);

		printf("path (%d -> %d): ", v1->data, v2->data);
		print_path(v1, v2);
		printf("\n");
	}

	return 0;
}
