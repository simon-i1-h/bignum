#include "bignat.h"

bignat
bignat_new(unsigned int n)
{
	return (bignat){.digit=n};
}

void
bignat_del(bignat n)
{
	(void)n;
}

int
bignat_eq(bool *dst, bignat x, bignat y)
{
	*dst = x.digit == y.digit;
	return 0;
}

int
bignat_ne(bool *dst, bignat x, bignat y)
{
	*dst = x.digit != y.digit;
	return 0;
}

int
bignat_add(bignat *sum, bignat x, bignat y)
{
	*sum = (bignat){
		.digit=x.digit + y.digit
	};
	return 0;
}

int
bignat_sub(bignat *diff, bignat x, bignat y)
{
	if (x.digit < y.digit) {
		return 1;
	}

	*diff = (bignat){
		.digit=x.digit - y.digit
	};
	return 0;
}

int
bignat_mul(bignat *prod, bignat x, bignat y)
{
	*prod = (bignat){
		.digit=x.digit * y.digit
	};
	return 0;
}

int
bignat_div(bignat *quot, bignat x, bignat y)
{
	if (y.digit == 0) {
		return 1;
	}

	*quot = (bignat){
		.digit=x.digit / y.digit
	};
	return 0;
}
