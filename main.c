#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bignum.h"

#define countof(a) (sizeof(a) / sizeof((a)[0]))

/* dump */

static dgtvec
dgtvec_new(uint32_t *digits, size_t ndigits)
{
	dgtvec v;
	int err = dgtvec_init(&v, digits, ndigits);
	if (err != 0) {
		fprintf(stderr, "%s: errno: %d\n", __func__, err);
		exit(1);
	}

	return v;
}

void
dump_dgtvec(void)
{
	{
		dgtvec v = dgtvec_new_empty();
		dgtvec_dump(v);
		dgtvec_del(v);
	}
	{
		uint32_t digit = 1;
		dgtvec v = dgtvec_new(&digit, 1);
		dgtvec_dump(v);
		dgtvec_del(v);
	}
	{
		uint32_t digits[] = {3, 1, 2};
		dgtvec v = dgtvec_new(digits, countof(digits));
		dgtvec_dump(v);
		dgtvec_del(v);
	}
}

/* test */

int nfailures = 0;
int nsuccesses = 0;

#define test_assert(expr) test_assert2(__FILE__, __LINE__, #expr, expr)
#define test_assert2(file, line, code, expr) do {			\
		if (!(expr)) {						\
			printf("%s:%d: test failed: %s\n", (file),	\
			       (line), (code));			\
			nfailures++;					\
		} else {						\
			nsuccesses++;					\
		}							\
	} while (0)

void
test_dgtvec_init(void)
{
	{
		dgtvec v;
		test_assert(dgtvec_init(&v, NULL, 0) == 0);
		test_assert(v.ndigits == 0);
		test_assert(v.digits == NULL);
	}
	{
		dgtvec v;
		test_assert(dgtvec_init(&v, (uint32_t[]){5}, 1) == 0);
		test_assert(v.ndigits == 1);
		test_assert(v.digits[0] == 5);
		free(v.digits);
	}
	{
		dgtvec v;
		uint32_t ds[] = {0, 9};
		test_assert(dgtvec_init(&v, ds, countof(ds)) == 0);
		test_assert(v.ndigits == 2);
		test_assert(v.digits[0] == 0);
		test_assert(v.digits[1] == 9);
		free(v.digits);
	}
	{
		dgtvec v;
		uint32_t ds[] = {0, 9, 0};
		test_assert(dgtvec_init(&v, ds, countof(ds)) == 0);
		test_assert(v.ndigits == 3);
		test_assert(v.digits[0] == 0);
		test_assert(v.digits[1] == 9);
		test_assert(v.digits[2] == 0);
		free(v.digits);
	}
}

void
test_dgtvec_new_empty(void)
{
	dgtvec v = dgtvec_new_empty();
	test_assert(v.ndigits == 0);
	test_assert(v.digits == NULL);
}

void
test_dgtvec_del(void)
{
	dgtvec v;
	test_assert(dgtvec_init(&v, (uint32_t[]){1}, 1) == 0);
	test_assert(v.ndigits == 1);
	test_assert(v.digits[0] == 1);
	dgtvec_del(v);
}

void
test_dgtvec_push(void)
{
	dgtvec v = dgtvec_new_empty();

	test_assert(dgtvec_push(&v, 0) == 0);
	test_assert(dgtvec_push(&v, 0) == 0);
	test_assert(dgtvec_push(&v, 1) == 0);
	test_assert(v.ndigits == 3);
	test_assert(v.digits[0] == 0);
	test_assert(v.digits[1] == 0);
	test_assert(v.digits[2] == 1);

	dgtvec_del(v);
}

void
test_dgtvec_pop(void)
{
	dgtvec v = dgtvec_new_empty();

	test_assert(dgtvec_push(&v, 3) == 0);
	test_assert(dgtvec_push(&v, 2) == 0);
	test_assert(dgtvec_push(&v, 1) == 0);
	test_assert(dgtvec_pop(&v) == 1);
	test_assert(v.ndigits == 2);
	test_assert(dgtvec_pop(&v) == 2);
	test_assert(v.ndigits == 1);
	test_assert(dgtvec_pop(&v) == 3);
	test_assert(v.ndigits == 0);

	dgtvec_del(v);
}

void
test_bignat_init(void)
{
	{
		bignat n;
		test_assert(bignat_init(&n, NULL, 0) == 0);
		test_assert(n.ndigits == 0);
		test_assert(n.digits == NULL);
	}
	{
		bignat n;
		test_assert(bignat_init(&n, (uint32_t[]){42}, 1) == 0);
		test_assert(n.ndigits == 1);
		test_assert(n.digits[0] == 42);
		free(n.digits);
	}
	{
		bignat n;
		uint32_t ds[] = {3, 55};
		test_assert(bignat_init(&n, ds, countof(ds)) == 0);
		test_assert(n.ndigits == 2);
		test_assert(n.digits[0] == 3);
		test_assert(n.digits[1] == 55);
		free(n.digits);
	}
	{
		bignat n;
		uint32_t ds[] = {3, 55, 0};
		test_assert(bignat_init(&n, ds, countof(ds)) == EINVAL);
	}
	{
		bignat n;
		uint32_t ds[] = {3, 55, 0, 0, 0};
		test_assert(bignat_init(&n, ds, countof(ds)) == EINVAL);
	}
}

void
test_bignat_from_digit(void)
{
	{
		bignat n;
		test_assert(bignat_from_digit(&n, 0) == 0);
		test_assert(n.ndigits == 0);
		test_assert(n.digits == NULL);
	}
	{
		bignat n;
		test_assert(bignat_from_digit(&n, 9) == 0);
		test_assert(n.ndigits == 1);
		test_assert(n.digits[0] == 9);
		free(n.digits);
	}
}

void
test_bignat_copy(void)
{
	{
		bignat src, dst;
		test_assert(bignat_init(&src, NULL, 0) == 0);
		test_assert(bignat_copy(&dst, src) == 0);
		test_assert(dst.ndigits == 0);
		test_assert(dst.digits == NULL);
	}
	{
		bignat src, dst;
		test_assert(bignat_init(&src, (uint32_t[]){95, 3}, 2) == 0);
		test_assert(bignat_copy(&dst, src) == 0);
		test_assert(dst.ndigits == 2);
		test_assert(dst.digits[0] == 95);
		test_assert(dst.digits[1] == 3);
		bignat_del(src);
		bignat_del(dst);
	}
}

/* for memory leak detection */
void
test_bignat_del(void)
{
	bignat n;
	test_assert(bignat_from_digit(&n, 1) == 0);
	test_assert(n.ndigits == 1);
	test_assert(n.digits[0] == 1);
	bignat_del(n);
}

void
test_bignat_cmp(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);
		test_assert(bignat_cmp(x, y) == 0);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);
		test_assert(bignat_cmp(x, y) == -1);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);
		test_assert(bignat_cmp(x, y) == 1);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		uint32_t xds[] = {0, 9, 1}, yds[] = {0, 9, 1};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_cmp(x, y) == 0);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		uint32_t xds[] = {0, 9, 1}, yds[] = {0, 0, 2};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_cmp(x, y) == -1);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		uint32_t xds[] = {1, 0, 3}, yds[] = {2, 9, 2};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_cmp(x, y) == 1);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		uint32_t xds[] = {0, 2}, yds[] = {0, 0, 1};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_cmp(x, y) == -1);
		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		uint32_t xds[] = {0, 0, 1}, yds[] = {0, 2};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_cmp(x, y) == 1);
		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_eq(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(bignat_eq(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(!bignat_eq(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_ne(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(bignat_ne(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(!bignat_ne(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_lt(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(!bignat_lt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(bignat_lt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(!bignat_lt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_gt(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(!bignat_gt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(!bignat_gt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(bignat_gt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_le(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(bignat_le(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(bignat_le(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(!bignat_le(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_ge(void)
{
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(bignat_ge(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);

		test_assert(!bignat_ge(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);

		test_assert(bignat_ge(x, y));

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_add(void)
{
	{
		bignat x, y, sum, expected;
		test_assert(bignat_from_digit(&x, 0) == 0);
		test_assert(bignat_from_digit(&y, 0) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);
		test_assert(bignat_from_digit(&expected, 3) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 1) == 0);
		test_assert(bignat_from_digit(&expected, 3) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		uint32_t xds[] = {1, 2, 3}, yds[] = {4, 5, 6},
			eds[] = {5, 7, 9};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		uint32_t xds[] = {0, 10, 1}, yds[] = {0, UINT32_MAX - 4, 1},
			eds[] = {0, 5, 3};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		uint32_t eds[] = {UINT32_MAX - 1, 1};
		test_assert(bignat_from_digit(&x, UINT32_MAX) == 0);
		test_assert(bignat_from_digit(&y, UINT32_MAX) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
	{
		bignat x, y, sum, expected;
		uint32_t xds[] = {0, 1},
			yds[] = {0, UINT32_MAX, UINT32_MAX},
			eds[] = {0, 0, 0, 1};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
		bignat_del(expected);
	}
}

void
test_bignat_sub(void)
{
	{
		bignat x, y, diff, expected;
		test_assert(bignat_from_digit(&x, 0) == 0);
		test_assert(bignat_from_digit(&y, 0) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		test_assert(bignat_from_digit(&x, 3) == 0);
		test_assert(bignat_from_digit(&y, 2) == 0);
		test_assert(bignat_from_digit(&expected, 1) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		test_assert(bignat_from_digit(&x, 3) == 0);
		test_assert(bignat_from_digit(&y, 3) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff;
		test_assert(bignat_from_digit(&x, 2) == 0);
		test_assert(bignat_from_digit(&y, 3) == 0);

		test_assert(bignat_sub(&diff, x, y) == EDOM);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y, diff, expected;
		uint32_t xds[] = {9, 8, 7}, yds[] = {1, 2, 3},
			eds[] = {8, 6, 4};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		uint32_t xds[] = {0, 0, 2}, yds[] = {0, 2},
			eds[] = {0, UINT32_MAX - 1, 1};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		uint32_t xds[] = {0, 0, 1}, yds[] = {0, 2},
			eds[] = {0, UINT32_MAX - 1};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		uint32_t xds[] = {0, 0, 1}, yds[] = {1},
			eds[] = {UINT32_MAX, UINT32_MAX};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_init(&expected, eds, countof(eds)) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff, expected;
		uint32_t xds[] = {UINT32_MAX - 1, 1, UINT32_MAX},
			yds[] = {UINT32_MAX - 1, 1, UINT32_MAX};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
		bignat_del(expected);
	}
	{
		bignat x, y, diff;
		uint32_t xds[] = {UINT32_MAX, UINT32_MAX, 1},
			yds[] = {0, 0, UINT32_MAX};
		test_assert(bignat_init(&x, xds, countof(xds)) == 0);
		test_assert(bignat_init(&y, yds, countof(yds)) == 0);

		test_assert(bignat_sub(&diff, x, y) == EDOM);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_mul(void)
{
	{
		bignat x, y, prod, expected;
		test_assert(bignat_from_digit(&x, 3) == 0);
		test_assert(bignat_from_digit(&y, 4) == 0);
		test_assert(bignat_from_digit(&expected, 12) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
		bignat_del(expected);
	}
	{
		bignat x, y, prod, expected;
		test_assert(bignat_from_digit(&x, 4) == 0);
		test_assert(bignat_from_digit(&y, 3) == 0);
		test_assert(bignat_from_digit(&expected, 12) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
		bignat_del(expected);
	}
	{
		bignat x, y, prod, expected;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 0) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
		bignat_del(expected);
	}
	{
		bignat x, y, prod, expected;
		test_assert(bignat_from_digit(&x, 0) == 0);
		test_assert(bignat_from_digit(&y, 0) == 0);
		test_assert(bignat_from_digit(&expected, 0) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
		bignat_del(expected);
	}
}

void
test_bignat_div(void)
{
	{
		bignat x, y, quot, expected;
		test_assert(bignat_from_digit(&x, 13) == 0);
		test_assert(bignat_from_digit(&y, 3) == 0);
		test_assert(bignat_from_digit(&expected, 4) == 0);

		test_assert(bignat_div(&quot, x, y) == 0);
		test_assert(bignat_eq(quot, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(quot);
		bignat_del(expected);
	}
	{
		bignat x, y, quot;
		test_assert(bignat_from_digit(&x, 1) == 0);
		test_assert(bignat_from_digit(&y, 0) == 0);

		test_assert(bignat_div(&quot, x, y) == EDOM);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bigint_init(void)
{
	{
		bigint i;
		test_assert(bigint_init(&i, 0, NULL, 0) == 0);
		test_assert(i.sign == 0);
		test_assert(i.abs.ndigits == 0);
		test_assert(i.abs.digits == NULL);
	}
	{
		bigint i;
		test_assert(bigint_init(&i, 1, (uint32_t[]){1}, 1) == 0);
		test_assert(i.sign == 1);
		test_assert(i.abs.ndigits == 1);
		test_assert(i.abs.digits[0] == 1);
		free(i.abs.digits);
	}
	{
		bigint i;
		test_assert(bigint_init(&i, -1, (uint32_t[]){1}, 1) == 0);
		test_assert(i.sign == -1);
		test_assert(i.abs.ndigits == 1);
		test_assert(i.abs.digits[0] == 1);
		free(i.abs.digits);
	}
	{
		bigint i;
		test_assert(bigint_init(&i, 1, NULL, 0) == EINVAL);
	}
	{
		bigint i;
		test_assert(bigint_init(&i, -1, NULL, 0) == EINVAL);
	}
	{
		bigint i;
		test_assert(bigint_init(&i, 0, (uint32_t[]){1}, 1) == EINVAL);
	}
	{
		bigint i;
		uint32_t ds[] = {0, 0, 5};
		test_assert(bigint_init(&i, -1, ds, countof(ds)) == 0);
		test_assert(i.sign == -1);
		test_assert(i.abs.ndigits == 3);
		test_assert(i.abs.digits[0] == 0);
		test_assert(i.abs.digits[1] == 0);
		test_assert(i.abs.digits[2] == 5);
		free(i.abs.digits);
	}
	{
		bigint i;
		uint32_t ds[] = {0};
		test_assert(bigint_init(&i, 0, ds, countof(ds)) == EINVAL);
	}
}

void
test_bigint_from_digit(void)
{
	{
		bigint i;
		test_assert(bigint_from_digit(&i, 0) == 0);
		test_assert(i.sign == 0);
		test_assert(i.abs.ndigits == 0);
		test_assert(i.abs.digits == NULL);
	}
	{
		bigint i;
		test_assert(bigint_from_digit(&i, 1) == 0);
		test_assert(i.sign == 1);
		test_assert(i.abs.ndigits == 1);
		test_assert(i.abs.digits[0] == 1);
		free(i.abs.digits);
	}
	{
		bigint i;
		test_assert(bigint_from_digit(&i, -1) == 0);
		test_assert(i.sign == -1);
		test_assert(i.abs.ndigits == 1);
		test_assert(i.abs.digits[0] == 1);
		free(i.abs.digits);
	}
}

void
test_bigint_del(void)
{
	bigint i;
	test_assert(bigint_from_digit(&i, 1) == 0);
	test_assert(i.sign == 1);
	test_assert(i.abs.ndigits == 1);
	test_assert(i.abs.digits[0] == 1);
	bigint_del(i);
}

void
test_bigint_eq(void)
{
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 1) == 0);
		test_assert(bigint_from_digit(&y, 1) == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, -1) == 0);
		test_assert(bigint_from_digit(&y, -1) == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 0) == 0);
		test_assert(bigint_from_digit(&y, 0) == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 1) == 0);
		test_assert(bigint_from_digit(&y, 2) == 0);

		test_assert(!bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, -1) == 0);
		test_assert(bigint_from_digit(&y, -2) == 0);

		test_assert(!bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
}

void
test_bigint_ne(void)
{
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 1) == 0);
		test_assert(bigint_from_digit(&y, 1) == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, -1) == 0);
		test_assert(bigint_from_digit(&y, -1) == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 0) == 0);
		test_assert(bigint_from_digit(&y, 0) == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, 1) == 0);
		test_assert(bigint_from_digit(&y, 2) == 0);

		test_assert(bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x, y;
		test_assert(bigint_from_digit(&x, -1) == 0);
		test_assert(bigint_from_digit(&y, -2) == 0);

		test_assert(bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
}

int
main(int argc, char **argv)
{
	if (argc > 1 && strcmp(argv[1], "--dumper") == 0) {
		dump_dgtvec();
		return 0;
	}

	/* dgtvec */
	test_dgtvec_init();
	test_dgtvec_new_empty();
	test_dgtvec_del();
	test_dgtvec_push();
	test_dgtvec_pop();

	/* bignat */
	test_bignat_init();
	test_bignat_from_digit();
	test_bignat_copy();
	test_bignat_del();
	test_bignat_cmp();
	test_bignat_eq();
	test_bignat_ne();
	test_bignat_lt();
	test_bignat_gt();
	test_bignat_le();
	test_bignat_ge();
	test_bignat_add();
	test_bignat_sub();
	test_bignat_mul();
	test_bignat_div();

	/* bigint */
	test_bigint_init();
	test_bigint_from_digit();
	test_bigint_del();
	test_bigint_eq();
	test_bigint_ne();

	printf("successes: %d\n", nsuccesses);
	printf("failures: %d\n", nfailures);

	return !!nfailures;
}
