#include <stdio.h>

#define DIGIT (10000 * 100)

struct bignum {
	char num[DIGIT];
	int size;
	bool sign;
};

void dump_bignum(struct bignum *big)
{
	int i = big->size;

	while (i--) {
		printf("%d", big->num[i]);
	}
	printf("\n");
}

/*
 * 2^n
 * T(n)=T(n/2)*T(n/2)*f(n)
 * f(n) = 1, n%2==0
 * f(n) = 2, n%2==1
 */
void bignum_exponent(struct bignum *out, int n)
{
}

void bignum_multi(struct bignum *out, struct bignum *in1, struct bignum *in2)
{
}

void bignum_add(struct bignum *sum, struct bignum *operand1, struct bignum *operand2)
{
	struct bignum *op = NULL;
	int carry = 0;
	int size1, size2;
	int i = 0;

	size1 = operand1->size;
	size2 = operand1->size;

	while (size1 && size2) {
		r = operand1->num[i] + operand2->num[i] + carry;
		if (r >= 10) {
			carry = 1;
			r = r % 10;
		}
		sum->num[i] = r;

		i++;
		size1--;
		size2--;
	}

	if (size1)
		op = operand1;
	else if (size2)
		op = operand2;

	if (op) {
		
	}
}

int main()
{
	struct bignum big1, big2, result;
	int i;

	i = 1000;


	return 0;
}
