#include <stdio.h>

#define STACKSIZE 100

struct element {
	int data;
	int min;
};

struct stack {
	struct element e[STACKSIZE];
	int size;
};

static int top(struct stack *s)
{
	return s->e[s->size - 1].data;
}

static int min(struct stack *s)
{
	return s->e[s->size - 1].min;
}

static int pop(struct stack *s)
{
	int val;

	if (!s->size) {
		printf("stack empty\n");
		return -1;
	}

	val = s->e[--s->size].data;
	return val;
}

static void push(struct stack *s, int data)
{
	if (s->size == STACKSIZE) {	
		printf("stack full\n");
		return;
	}

	s->e[s->size].data = data;

	if (!s->size || data < min(s))
		s->e[s->size].min = data;
	else
		s->e[s->size].min = min(s);

	s->size++;
}

int main()
{
	struct stack s;

	s.size = 0;
	push(&s, 9);
	push(&s, 20);
	push(&s, 10);
	push(&s, 2);
	push(&s, 8);
	push(&s, 5);
	push(&s, 2);

	printf("min: %d\n", min(&s));

	pop(&s);
	printf("min: %d\n", min(&s));

	pop(&s);
	printf("min: %d\n", min(&s));

	pop(&s);
	printf("min: %d\n", min(&s));

	pop(&s);
	printf("min: %d\n", min(&s));

	return 0;
}
