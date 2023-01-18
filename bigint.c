#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

static int
bigint_init(bigint *int_, int sign, uint32_t *digits, size_t ndigits)
{

	if (ndigits == 0) {
		if (sign != 0) {
			return EINVAL;
		}
	} else {
		if (sign == 0) {
			return EINVAL;
		}
	}

	bignat abs;
	int err = bignat_init(&abs, digits, ndigits);
	if (err != 0) {
		return err;
	}

	*int_ = (bigint){
		.sign=sign,
		.abs=abs
	};

	return 0;
}

bigint
bigint_new(int sign, uint32_t *digits, size_t ndigits)
{
	bigint int_;
	int err = bigint_init(&int_, sign, digits, ndigits);

	if (err == EINVAL) {
		fprintf(stderr, "EINVAL\n");
		exit(1);
	} else if (err == ENOMEM) {
		fprintf(stderr, "ENOMEM\n");
		exit(1);
	} else if (err != 0) {
		fprintf(stderr, "unexpected error\n");
		exit(1);
	}

	return int_;
}

bigint
bigint_from_digit(int32_t x)
{
	if (x == 0) {
		return bigint_new(0, NULL, 0);
	}

	int sign = x < 0 ? -1 : 1;
	uint32_t abs = x < 0 ? (int64_t)x * -1 : x;
	return bigint_new(sign, &abs, 1);
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
