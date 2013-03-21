#include <stdio.h>
#include <string.h>

static void revert_str(char *str)
{
	char *p1, *p2;
	char c;

	p1 = str;	
	p2 = str + strlen(str) - 1;

	while (p1 < p2) {
		c = *p1;	
		*p1 = *p2;
		*p2 = c;

		p1++;
		p2--;
	}
}

int main()
{
	char str[] = "abcdef";

	printf("%s\n", str);
	revert_str(str);
	printf("%s\n", str);

	return 0;
}
