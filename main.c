#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

#define test_assert(expr) test_assert2(__FILE__, __LINE__, #expr, expr)
#define test_assert2(file, line, code, expr) do {			\
		if (!(expr)) {						\
			printf("%s:%d: test failed: %s\n", (file),	\
			       (line), (code));			\
			exit(1);					\
		}							\
	} while (0)

void
test_bignat_eq(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_eq(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
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
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bool b;

		test_assert(bignat_ne(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_ne(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_lt(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_lt(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bool b;

		test_assert(bignat_lt(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_lt(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_gt(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_gt(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bool b;

		test_assert(bignat_gt(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_gt(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_le(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_le(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bool b;

		test_assert(bignat_le(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_le(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_ge(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_ge(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bool b;

		test_assert(bignat_ge(&b, x, y) == 0);
		test_assert(!b);

		bignat_del(x);
		bignat_del(y);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(1);
		bool b;

		test_assert(bignat_ge(&b, x, y) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bignat_add(void)
{
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(2);
		bignat sum;
		bool b;

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(&b, sum, bignat_from_a_digit(3)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(1);
		bignat sum;
		bool b;

		test_assert(bignat_add(&sum, x, y) == 0);
		test_assert(bignat_eq(&b, sum, bignat_from_a_digit(3)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(sum);
	}
}

void
test_bignat_sub(void)
{
	{
		bignat x = bignat_from_a_digit(3);
		bignat y = bignat_from_a_digit(2);
		bignat diff;
		bool b;

		test_assert(bignat_sub(&diff, x, y) == 0);
		test_assert(bignat_eq(&b, diff, bignat_from_a_digit(1)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(diff);
	}
	{
		bignat x = bignat_from_a_digit(2);
		bignat y = bignat_from_a_digit(3);
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
		bignat x = bignat_from_a_digit(3);
		bignat y = bignat_from_a_digit(4);
		bignat prod;
		bool b;

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(&b, prod, bignat_from_a_digit(12)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(0);
		bignat prod;
		bool b;

		test_assert(bignat_mul(&prod, x, y) == 0);
		test_assert(bignat_eq(&b, prod, bignat_from_a_digit(0)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(prod);
	}
}

void
test_bignat_div(void)
{
	{
		bignat x = bignat_from_a_digit(13);
		bignat y = bignat_from_a_digit(3);
		bignat quot;
		bool b;

		test_assert(bignat_div(&quot, x, y) == 0);
		test_assert(bignat_eq(&b, quot, bignat_from_a_digit(4)) == 0);
		test_assert(b);

		bignat_del(x);
		bignat_del(y);
		bignat_del(quot);
	}
	{
		bignat x = bignat_from_a_digit(1);
		bignat y = bignat_from_a_digit(0);
		bignat quot;

		test_assert(bignat_div(&quot, x, y) == 1);

		bignat_del(x);
		bignat_del(y);
	}
}

void
test_bigint_eq(void)
{
	{
		bigint x = bigint_new(1, 1);
		bigint y = bigint_new(1, 1);
		bool b;

		test_assert(bigint_eq(&b, x, y) == 0);
		test_assert(b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(-1, 1);
		bigint y = bigint_new(-1, 1);
		bool b;

		test_assert(bigint_eq(&b, x, y) == 0);
		test_assert(b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(0, 0);
		bigint y = bigint_new(0, 0);
		bool b;

		test_assert(bigint_eq(&b, x, y) == 0);
		test_assert(b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(1, 1);
		bigint y = bigint_new(1, 2);
		bool b;

		test_assert(bigint_eq(&b, x, y) == 0);
		test_assert(!b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(-1, 1);
		bigint y = bigint_new(-1, 2);
		bool b;

		test_assert(bigint_eq(&b, x, y) == 0);
		test_assert(!b);

		bigint_del(x);
		bigint_del(y);
	}
}

void
test_bigint_ne(void)
{
	{
		bigint x = bigint_new(1, 1);
		bigint y = bigint_new(1, 1);
		bool b;

		test_assert(bigint_ne(&b, x, y) == 0);
		test_assert(!b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(-1, 1);
		bigint y = bigint_new(-1, 1);
		bool b;

		test_assert(bigint_ne(&b, x, y) == 0);
		test_assert(!b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(0, 0);
		bigint y = bigint_new(0, 0);
		bool b;

		test_assert(bigint_ne(&b, x, y) == 0);
		test_assert(!b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(1, 1);
		bigint y = bigint_new(1, 2);
		bool b;

		test_assert(bigint_ne(&b, x, y) == 0);
		test_assert(b);

		bigint_del(x);
		bigint_del(y);
	}
	{
		bigint x = bigint_new(-1, 1);
		bigint y = bigint_new(-1, 2);
		bool b;

		test_assert(bigint_ne(&b, x, y) == 0);
		test_assert(b);

		bigint_del(x);
		bigint_del(y);
	}
}

int
main(void)
{
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

	printf("done\n");
	return 0;
}
