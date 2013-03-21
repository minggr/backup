#include <stdio.h>

int mp(int n)
{
	int m = 1;

	while (n-- > 0)
		m = m * 2;
	return m-1;
}

int digit(int n)
{
	int i = 0;

	do {
		i++;
		n = n / 10;
	} while (n);

	return i;
}

int main()
{
	int i;
	int m;

	for (i = 1; i <= 32; i++) {
		m = mp(i);
		printf("%d: %d  %d\n", i, m, digit(m));
	}

	return 0;
}
