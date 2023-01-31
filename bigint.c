#include <errno.h>
#include <stdint.h>

#include "bignum.h"

int
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

int
bigint_from_digit(bigint *int_, int32_t x)
{
	if (x == 0) {
		return bigint_init(int_, 0, NULL, 0);
	}

	int sign = x < 0 ? -1 : 1;
	uint32_t abs = x < 0 ? (int64_t)x * -1 : x;
	return bigint_init(int_, sign, &abs, 1);
}

int
bigint_copy(bigint *dst, bigint src)
{
	return bigint_init(dst, src.sign, src.abs.digits, src.abs.ndigits);
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

bool
bigint_lt(bigint x, bigint y)
{
	return bigint_cmp(x, y) < 0;
}

bool
bigint_gt(bigint x, bigint y)
{
	return bigint_cmp(x, y) > 0;
}

bool
bigint_le(bigint x, bigint y)
{
	return bigint_cmp(x, y) <= 0;
}

bool
bigint_ge(bigint x, bigint y)
{
	return bigint_cmp(x, y) >= 0;
}

int
bigint_add(bigint *sum, bigint x, bigint y)
{
	int err = -1;
	bignat abs;

	if (x.sign == y.sign) {
		err = bignat_add(&abs, x.abs, y.abs);
		if (err != 0) {
			return err;
		}

		*sum = (bigint){
			.sign=x.sign,
			.abs=abs
		};
		return 0;
	}

	int cmpabs = bignat_cmp(x.abs, y.abs);

	if (cmpabs < 0) {
		err = bignat_sub(&abs, y.abs, x.abs);
		if (err != 0) {
			return err;
		}

		*sum = (bigint){
			.sign=y.sign,
			.abs=abs
		};
		return 0;
	}

	if (cmpabs > 0) {
		err = bignat_sub(&abs, x.abs, y.abs);
		if (err != 0) {
			return 0;
		}

		*sum = (bigint){
			.sign=x.sign,
			.abs=abs
		};
		return 0;
	}

	*sum = (bigint){
		.sign=0,
		.abs=bignat_new_zero()
	};
	return 0;
}

int
bigint_sub(bigint *diff, bigint x, bigint y)
{
	bigint neg_y = {
		.sign=y.sign * -1,
		.abs=y.abs
	};

	return bigint_add(diff, x, neg_y);
}
