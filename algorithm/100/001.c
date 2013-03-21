#include <stdio.h>

struct node {
	int data;
	struct node *left, *right;
};

static struct node n[7];
static struct node *root = &n[0];
static struct node tmp;

static void init()
{
	n[0].data = 10;
	n[1].data = 6;
	n[2].data = 14;
	n[3].data = 4;
	n[4].data = 8;
	n[5].data = 12;
	n[6].data = 16;

	n[0].left = &n[1];
	n[0].right = &n[2];
	n[1].left = &n[3];
	n[1].right = &n[4];
	n[2].left = &n[5];
	n[2].right = &n[6];

	tmp.data = 3;
	n[3].left = &tmp;
}

#if 0
static void tree2link(struct node *root, struct node **head, struct node **tail)
{
	struct node *h, *t;	

	if (root->left) {
		tree2link(root->left, &h, &t);	
		*head = h;
		root->left = t;
		if (t)
			t->right = root;
	} else
		*head = root;

	if (root->right) {
		tree2link(root->right, &h, &t);	
		*tail = t;
		root->right = h;
		if (h)
			h->left = root;
	} else
		*tail = root;
}

#else
static void tree2link(struct node *root, struct node **head, struct node **tail)
{
	struct node *h, *t;	

	if (!root) {
		*head = *tail = NULL;
		return;
	}

	tree2link(root->left, &h, &t);	
	if (h)
		*head = h;
	else
		*head = root;
	root->left = t;
	if (t)
		t->right = root;

	tree2link(root->right, &h, &t);	
	if (t)
		*tail = t;
	else
		*tail = root;
	root->right = h;
	if (h)
		h->left = root;
}
#endif

static void print_link(struct node *head)
{
	struct node *node = head;

	while (node) {
		printf("%d ", node->data);
		node = node->right;
	}
	printf("\n");
}

int main()
{
	struct node *h, *t;

	init();
	tree2link(root, &h, &t);
	print_link(h);

	return 0;
}
