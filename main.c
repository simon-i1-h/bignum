#include <stdio.h>
#include <stdlib.h>

#include "bignat.h"

#define test_assert(expr) test_assert2(__FILE__, __LINE__, #expr, expr)
#define test_assert2(file, line, code, expr) do {			\
		if (!(expr)) {						\
			printf("%s:%d: test failed: %s\n", (file), (line), \
			       (code));				\
			exit(1);					\
		}							\
	} while (0)

void
test_bignat_eq(void)
{
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(1);
		bool b;

		test_assert(bignat_eq(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(2);
		bool b;

		test_assert(bignat_eq(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_ne(void)
{
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(2);
		bool b;

		test_assert(bignat_ne(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(1);
		bool b;

		test_assert(bignat_ne(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_add(void)
{
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(2);
		bignat sum;

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(sum.digit == 3);

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
	{
		bignat x = bignat_new(2);
		bignat y = bignat_new(1);
		bignat sum;

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(sum.digit == 3);

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
}

void
test_bignat_sub(void)
{
	{
		bignat x = bignat_new(3);
		bignat y = bignat_new(2);
		bignat diff;

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(diff.digit == 1);

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
	}
	{
		bignat x = bignat_new(2);
		bignat y = bignat_new(3);
		bignat diff;

		test_assert(bignat_sub(&diff, x, y) == 1);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_mul(void)
{
	{
		bignat x = bignat_new(3);
		bignat y = bignat_new(4);
		bignat prod;

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(prod.digit == 12);

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(0);
		bignat prod;

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(prod.digit == 0);

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
}

void
test_bignat_div(void)
{
	{
		bignat x = bignat_new(13);
		bignat y = bignat_new(3);
		bignat quot;

		test_assert(bignat_div(&quot, x, y) == 0);
		test_assert(quot.digit == 4);

		bignat_del(x);
		bignat_del(y);
		bignat_del(quot);
	}
	{
		bignat x = bignat_new(1);
		bignat y = bignat_new(0);
		bignat quot;

		test_assert(bignat_div(&quot, x, y) == 1);

		bignat_del(x);
		bignat_del(y);
		bignat_del(quot);
	}
}

int
main(void)
{
	/* nat */
	test_bignat_eq();
	test_bignat_ne();
	test_bignat_add();
	test_bignat_sub();
	test_bignat_mul();
	test_bignat_div();

	printf("done\n");
	return 0;
}
