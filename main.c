#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bignum.h"

#define countof(a) (sizeof(a) / sizeof((a)[0]))

/* dump */

void
dump_dgtvec(void)
{
	{
		dgtvec v = dgtvec_new(NULL, 0);
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
test_dgtvec_push(void)
{
	dgtvec v = dgtvec_new(NULL, 0);

	dgtvec_push(&v, 0);
	dgtvec_push(&v, 0);
	dgtvec_push(&v, 1);
	test_assert(v.ndigits == 3);
	test_assert(v.digits[0] == 0);
	test_assert(v.digits[1] == 0);
	test_assert(v.digits[2] == 1);

	dgtvec_del(v);
}

void
test_dgtvec_pop(void)
{
	dgtvec v = dgtvec_new(NULL, 0);

	dgtvec_push(&v, 3);
	dgtvec_push(&v, 2);
	dgtvec_push(&v, 1);
	test_assert(dgtvec_pop(&v) == 1);
	test_assert(v.ndigits == 2);
	test_assert(dgtvec_pop(&v) == 2);
	test_assert(v.ndigits == 1);
	test_assert(dgtvec_pop(&v) == 3);
	test_assert(v.ndigits == 0);

	dgtvec_del(v);
}

void
test_bignat_eq(void)
{
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

		test_assert(bignat_eq(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

		test_assert(bignat_ne(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

		test_assert(!bignat_lt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

		test_assert(bignat_lt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

		test_assert(!bignat_gt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

		test_assert(!bignat_gt(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

		test_assert(bignat_le(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

		test_assert(bignat_le(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

		test_assert(bignat_ge(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);

		test_assert(!bignat_ge(x, y));

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x, y;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);

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
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&expected, (uint32_t[]){3}, 1) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
	{
		bignat x, y, sum, expected;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&expected, (uint32_t[]){3}, 1) == 0);

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(sum, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
}

void
test_bignat_sub(void)
{
	{
		bignat x, y, diff, expected;
		test_assert(bignat_init(&x, (uint32_t[]){3}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&expected, (uint32_t[]){1}, 1) == 0);

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(diff, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
	}
	{
		bignat x, y, diff;
		test_assert(bignat_init(&x, (uint32_t[]){2}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){3}, 1) == 0);

		test_assert(bignat_sub(&diff, x, y) == 1);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_mul(void)
{
	{
		bignat x, y, prod, expected;
		test_assert(bignat_init(&x, (uint32_t[]){3}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){4}, 1) == 0);
		test_assert(bignat_init(&expected, (uint32_t[]){12}, 1) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
	{
		bignat x, y, prod, expected;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, NULL, 0) == 0);
		test_assert(bignat_init(&expected, NULL, 0) == 0);

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(prod, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
}

void
test_bignat_div(void)
{
	{
		bignat x, y, quot, expected;
		test_assert(bignat_init(&x, (uint32_t[]){13}, 1) == 0);
		test_assert(bignat_init(&y, (uint32_t[]){3}, 1) == 0);
		test_assert(bignat_init(&expected, (uint32_t[]){4}, 1) == 0);

		test_assert(bignat_div(&quot, x, y) == 0);
		test_assert(bignat_eq(quot, expected));

		bignat_del(x);
		bignat_del(y);
		bignat_del(quot);
	}
	{
		bignat x, y, quot;
		test_assert(bignat_init(&x, (uint32_t[]){1}, 1) == 0);
		test_assert(bignat_init(&y, NULL, 0) == 0);

		test_assert(bignat_div(&quot, x, y) == 1);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bigint_eq(void)
{
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 0, NULL, 0);
		test_assert(err == 0);
		err = bigint_init(&y, 0, NULL, 0);
		test_assert(err == 0);

		test_assert(bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, 1, (uint32_t[]){2}, 1);
		test_assert(err == 0);

		test_assert(!bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, -1, (uint32_t[]){2}, 1);
		test_assert(err == 0);

		test_assert(!bigint_eq(x, y));

		bigint_del(x);
		bigint_del(y);
	}
}

void
test_bigint_ne(void)
{
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 0, NULL, 0);
		test_assert(err == 0);
		err = bigint_init(&y, 0, NULL, 0);
		test_assert(err == 0);

		test_assert(!bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, 1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, 1, (uint32_t[]){2}, 1);
		test_assert(err == 0);

		test_assert(bigint_ne(x, y));

		bigint_del(x);
		bigint_del(y);
	}
	{
		int err;
		bigint x, y;

		err = bigint_init(&x, -1, (uint32_t[]){1}, 1);
		test_assert(err == 0);
		err = bigint_init(&y, -1, (uint32_t[]){2}, 1);
		test_assert(err == 0);

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
	test_dgtvec_push();
	test_dgtvec_pop();

	/* bignat */
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
	test_bigint_eq();
	test_bigint_ne();

	printf("successes: %d\n", nsuccesses);
	printf("failures: %d\n", nfailures);

	return !!nfailures;
}
