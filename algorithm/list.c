struct node {
	struct node *next;
	int data;
};

void remove(struct node **head, int data)
{
	struct node *prev, *next;

	prev = next = *head;
	while (next) {
		if (next->data == data) {
			if (next == head)
				*head = next->next;
			else
				prev->next = next->next;
			free(next);

			break;
		}

		prev = next->next;	
	}
}

void remove(struct node **head, int data)
{
	struct node **n = head;

	while (*n) {
		if ((*n)->data == data) {
			struct node *node;

			node = *n;
			*n = (*n)->next;

			free(node);
		}
	}
}
