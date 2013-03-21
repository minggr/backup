#include <stdio.h>

#define NUM 10

struct queen {
	int row[NUM];
	int n;
};

static int reject(struct queen *q)
{
	int i = q->n - 1;
	int last = q->row[i];
	int col1, col2;

	col1 = col2= last;

	if (!i)
		return 0;

	i = i - 1;
	while (i >= 0) {
		if (q->row[i] == last)
			return 1;
		col1--;
		col2++;
		if (q->row[i] == col1 || q->row[i] == col2)
			return 1;
		i--;
	}

	return 0;
}

static int accept(struct queen *q)
{
	return q->n == NUM;
}

static struct queen *first(struct queen *q)
{
	if (q->n == NUM)
		return NULL;
	q->row[q->n] = 0; 
	q->n++;
	return q;
}

static struct queen *next(struct queen *q)
{
	if (q->row[q->n - 1] == (NUM - 1))
		return NULL;
	q->row[q->n - 1] = q->row[q->n - 1] + 1;
	return q;
}

static struct queen *root(struct queen *q, int col)
{
	q->row[0] = col;
	q->n = 1;

	return q;
}

static void output(struct queen *q)
{
	int i;

	for (i = 0; i < q->n; i++)
		printf("%d ", q->row[i]);
	printf("\n");
}

static void bt(struct queen *q)
{
	struct queen *nq;
	int i;

	if (reject(q))
		return;
	if (accept(q))
		output(q);
	
	nq = first(q);
	while (nq) {
		i = nq->n;
		bt(nq);
		nq->n = i;
		nq = next(nq);
	}
}

int main()
{
	struct queen q;
	int i;

	for (i = 0; i < NUM; i++)
		bt(root(&q, i));

	return 0;
}
