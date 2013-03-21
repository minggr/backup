#include <stdio.h>

int mycount;

#define NUM 20
struct candidate {
	int data_orig[NUM];
	int data[NUM];
	int n;
	int sum;
};

void output(struct candidate *c)
{
	int i;

	for (i = 0; i < c->n; i++)
		printf("%d ", c->data[i]);
	printf("\n");
}

int count(struct candidate *c, int i)
{
	int j;
	int s = 0;

	for (j = 0; j < c->n; j++)
		if (c->data[j] == c->data_orig[i])
			s++;
	return s;
}

int reject(struct candidate *c)
{
	int i;

	if (c->sum > NUM)
		return 1;

	if (c->n < NUM) {
		for (i = 0; i < c->n; i++) {
			if (c->data[i] < count(c, i))	
				return 1;
		}
	} else {
		for (i = 0; i < c->n; i++) {
			if (c->data[i] != count(c, i))	
				return 1;
		}
	}

	return 0;
}

int accept(struct candidate *c)
{
	return c->n == NUM;
}

struct candidate *first(struct candidate *c)
{
	if (c->n == NUM)
		return NULL;
	
	c->data[c->n] = 0;
	c->n++;
	return c;
}

struct candidate *next(struct candidate *c)
{
	int i = c->n - 1;

	if (c->data[i] == NUM)
		return NULL;

	c->data[i] += 1;
	c->sum += 1;

	if (reject(c))
		return NULL;

	return c;
}


struct candidate *root(struct candidate *c, int i)
{
	c->data[0] = i;
	c->n = 1;
	c->sum = i;
}

void backtrack(struct candidate *r)
{
	struct candidate *c;
	int n;
	int sum;

	mycount++;

	if (reject(r)) {
		return;
	}
	if (accept(r))
		output(r);

	c = first(r);
	while (c) {
		n = r->n;
		sum = r->sum;
		backtrack(c);
		r->n = n;
		r->sum = sum;
		c = next(c);
	}
}

void brute_force()
{
	int a[NUM] = { 0 };
	int b[NUM] = { 0 };
	int i;

	for (i = 0; i < NUM; i++)
		a[i] = i;

	
}

int main()
{
	struct candidate cand, *c;	
	int i;

	c = &cand;
	c->n = 0;
	c->sum = 0;
	for (i = 0; i < NUM; i++)
		c->data_orig[i] = i;

	for (i = 0; i <= NUM; i++)
		backtrack(root(c, i));	

	printf("mycount=%d\n", mycount);

	return 0;
}
