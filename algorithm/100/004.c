#include <stdio.h>

struct node {
	int data;
	struct node *left;
	struct node *right;
	struct node *next;
	int sum;
};

#define RESULT 22

static struct node *stack;
static struct node n[5];

static void push(struct node **stack, struct node *n)
{
	int prev_sum;

	//printf("push %d\n", n->data);

	if (!*stack)
		prev_sum = 0;
	else
		prev_sum = (*stack)->sum;
	n->next = *stack;
	*stack = n;
	n->sum = prev_sum + n->data;
}

static struct node *pop(struct node **stack)
{
	struct node *n = *stack;

	if (!n)
		return NULL;

	*stack = n->next;
	//printf("pop %d\n", n->data);
	return n;
}

static struct node *top(struct node *stack)
{
	return stack;
}

static void output_stack_node(struct node *stack)
{
	while (stack) {
		printf("%d ", stack->data);
		stack = stack->next;
	}
	printf("\n");
}

static void backtrack(struct node *n)
{
	if (!n)
		return;

	push(&stack, n);
	backtrack(n->left);
	backtrack(n->right);
	if (!n->left && !n->right && n->sum == RESULT)
		output_stack_node(stack);
	pop(&stack);
}

int main()
{
	n[0].data = 10;
	n[1].data = 5;
	n[2].data = 12;
	n[3].data = 4;
	n[4].data = 7;

	n[0].left = &n[1];
	n[0].right = &n[2];
	n[1].left = &n[3];
	n[1].right = &n[4];

	backtrack(&n[0]);

	return 0;
}
