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
		.abs=bignat_from_a_digit(abs)
	};
}

void
bigint_del(bigint int_)
{
	bignat_del(int_.abs);
}

static int
bigint_cmp(int *dst, bigint x, bigint y)
{
	if (x.sign < y.sign) {
		*dst = -1;
		return 0;
	}

	if (x.sign > y.sign) {
		*dst = 1;
		return 0;
	}

	/* x.sign == y.sign */

	if (x.sign < 0) {
		return bignat_cmp(dst, y.abs, x.abs);
	}

	if (x.sign > 0) {
		return bignat_cmp(dst, x.abs, y.abs);
	}

	/* x.sign == 0 && y.sign == 0 */

	*dst = 0;
	return 0;
}

int
bigint_eq(bool *dst, bigint x, bigint y)
{
	int cmp;
	int err = bigint_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp == 0;
	}
	return err;
}

int
bigint_ne(bool *dst, bigint x, bigint y)
{
	int cmp;
	int err = bigint_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp != 0;
	}
	return err;
}
