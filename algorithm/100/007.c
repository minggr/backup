#include <stdio.h>

struct node {
	int data;
	struct node *next;
};

struct node *circular(struct node *h)
{
	struct node *n1, *n2;
	struct node *meet_node = NULL;

	if (!h)
		return NULL;

	n1 = n2 = h;
	//n2 = h->next;
	//if (n2)
	//	n2 = n2->next;
	//printf("\n\n");
	while (n1 && n2) {
		n1 = n1->next;
		n2 = n2->next;
		if (n2)
			n2 = n2->next;
		//printf("%d, %d\n", n1->data, n2->data);
		if (!n1 || !n2)
			return NULL;
		if (n1 == n2) {
			meet_node = n1;
			break;
		}
	}

	if (!meet_node)
		return NULL;

	n1 = h;
	n2 = meet_node;
	//printf("%d, %d\n", n1->data, n2->data);	

	while (n1 != n2) {
		n1 = n1->next;
		n2 = n2->next;
	}

	return n1;
}

int cross_both_circular(struct node *h1, struct node *h2)
{
	struct node *c1, *c2;
	struct node *p;

	c1 = circular(h1);
	c2 = circular(h2);

	if (c1 == c2)
		return 1;

	p = c1->next;
	while (p != c1) {
		if (p == c2)
			return 1;
		p = p->next;
	}

	p = c2->next;
	while (p != c2) {
		if (p == c1)
			return 1;
		p = p->next;
	}

	return 0;
}

int cross_no_circular(struct node *h1, struct node *h2)
{
	while (h1) {
		if (!h1->next)
			break;

		h1 = h1->next;
	}

	while (h2) {
		if (!h2->next)
			break;

		h2 = h2->next;
	}

	return h1 == h2;
}

int cross(struct node *h1, struct node *h2)
{
	struct node *c1, *c2;

	c1 = circular(h1);
	c2 = circular(h2);

	//neither circular
	if (!c1 && !c2)
		return cross_no_circular(h1, h2);

	//one circular, another no
	if ((c1 && !c2) || (!c1 && c2))
		return 0;

	return cross_both_circular(h1, h2);
}

int length(struct node *h, struct node *last_node)
{
	int len = 0;

	while (h) {
		len++;	
		if (h == last_node)
			break;
		h = h->next;
	}

	return len;
}

struct node *last_node(struct node *h)
{
	struct node *c = circular(h);
	struct node *p = h;
	struct node *prev = p;
	int visited = 0;

	while (p) {
		if (p == c)
			visited = 1;
		prev = p;
		p = p->next;
		if (p == c && visited)
			break;
	}

	return prev;
}

struct node *first_cross_node(struct node *h1, struct node *h2)
{
	struct node *p1, *p2;
	struct node *last;
	int len1, len2;
	int i;

	if (!cross(h1, h2))
		return NULL;

	last = last_node(h1);
	len1 = length(h1, last);
	len2 = length(h2, last);
	printf("len1=%d, len2=%d\n", len1, len2);

	if (len1 > len2) {
		p1 = h1;
		p2 = h2;
		i = len1 - len2;
	} else {
		p1 = h2;
		p2 = h1;
		i = len2 - len1;
	}

	while (i--)
		p1 = p1->next;

	printf("p1=%d, p2=%d\n", p1->data, p2->data);

	while (p1 != p2) {
		p1 = p1->next;
		p2 = p2->next;
	}

	return p1;
}

#define NUM 100

int main()
{
	struct node n1[NUM], n2[NUM];
	struct node *cross_node;
	int i;

	for (i = 0; i < NUM - 1; i++) {
		n1[i].next = &n1[i+1];
		n1[i].data = i;
		n2[i].next = &n2[i+1];
		n2[i].data = i*10;
	}
	n1[i].next = NULL;
	n2[i].next = NULL;

	n1[5].next = &n1[2];
	n2[4].next = &n1[3];

	printf("circular: %d, %d\n", circular(n1)->data, circular(n2)->data);
	
	printf("cross: %d\n", cross(n1, n2));
	cross_node = first_cross_node(n2, n1);
	if (cross_node)	
		printf("first cross node: %d\n", cross_node->data);

	return 0;
}
