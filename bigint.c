#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

bigint
bigint_new(int sign, uint32_t abs)
{
	if (abs == 0) {
		if (sign != 0) {
			fprintf(stderr, "invalid sign\n");
			exit(1);
		}
	} else {
		if (sign == 0) {
			fprintf(stderr, "invalid sign\n");
			exit(1);
		}
	}

	return (bigint){
		.sign=sign,
		.abs=bignat_from_digit(abs)
	};
}

void
bigint_del(bigint int_)
{
	bignat_del(int_.abs);
}

static int
bigint_cmp(bigint x, bigint y)
{
	if (x.sign < y.sign) {
		return -1;
	}

	if (x.sign > y.sign) {
		return 1;
	}

	/* x.sign == y.sign */

	if (x.sign < 0) {
		return bignat_cmp(y.abs, x.abs);
	}

	if (x.sign > 0) {
		return bignat_cmp(x.abs, y.abs);
	}

	/* x.sign == 0 && y.sign == 0 */

	return 0;
}

bool
bigint_eq(bigint x, bigint y)
{
	return bigint_cmp(x, y) == 0;
}

bool
bigint_ne(bigint x, bigint y)
{
	return bigint_cmp(x, y) != 0;
}
