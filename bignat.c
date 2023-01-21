#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

static void
bignat_norm(bignat *nat)
{
	while (nat->ndigits > 0 && nat->digits[nat->ndigits - 1] == 0) {
		dgtvec_pop(nat);
	}
}

int
bignat_init(bignat *nat, uint32_t *digits, size_t ndigits)
{
	if (ndigits > 0 && digits[ndigits - 1] == 0) {
		return EINVAL;
	}

	return dgtvec_init(nat, digits, ndigits);
}

static bignat
bignat_new_zero(void)
{
	return dgtvec_new_empty();
}

int
bignat_from_digit(bignat *nat, uint32_t n)
{
	if (n == 0) {
		return dgtvec_init(nat, NULL, 0);
	}

	return dgtvec_init(nat, &n, 1);
}

void
bignat_del(bignat n)
{
	dgtvec_del(n);
}

int
bignat_cmp(bignat x, bignat y)
{
	if (x.ndigits < y.ndigits) {
		return -1;
	}

	if (x.ndigits > y.ndigits) {
		return 1;
	}

	/* x.ndigits == y.ndigits */

	for (size_t i = x.ndigits - 1; i < x.ndigits; i--) {
		if (x.digits[i] < y.digits[i]) {
			return -1;
		}

		if (x.digits[i] > y.digits[i]) {
			return 1;
		}
	}

	return 0;
}

bool
bignat_eq(bignat x, bignat y)
{
	return bignat_cmp(x, y) == 0;
}

bool
bignat_ne(bignat x, bignat y)
{
	return bignat_cmp(x, y) != 0;
}

bool
bignat_lt(bignat x, bignat y)
{
	return bignat_cmp(x, y) < 0;
}

bool
bignat_gt(bignat x, bignat y)
{
	return bignat_cmp(x, y) > 0;
}

bool
bignat_le(bignat x, bignat y)
{
	return bignat_cmp(x, y) <= 0;
}

bool
bignat_ge(bignat x, bignat y)
{
	return bignat_cmp(x, y) >= 0;
}

int
bignat_add(bignat *sum, bignat x, bignat y)
{
	if (x.ndigits < y.ndigits) {
		return bignat_add(sum, y, x);
	}

	/* x.ndigits >= y.ndigits */

	int err = -1;
	bignat tmp_sum = bignat_new_zero();
	uint32_t carry = 0;
	uint32_t y_digit;
	uint64_t sum_digit;

	for (size_t i = 0; i < x.ndigits; i++) {
		y_digit = i < y.ndigits ? y.digits[i] : 0;
		sum_digit = (uint64_t)x.digits[i] + (uint64_t)y_digit +
			(uint64_t)carry;
		carry = sum_digit >> 32;

		err = dgtvec_push(&tmp_sum, sum_digit & ~(uint32_t)0);
		if (err != 0) {
			goto fail;
		}
	}

	if (carry != 0) {
		err = dgtvec_push(&tmp_sum, carry);
		if (err != 0) {
			goto fail;
		}
	}

	*sum = tmp_sum;
	return 0;

fail:
	bignat_del(tmp_sum);
	return err;
}

int
bignat_sub(bignat *diff, bignat x, bignat y)
{
	if (bignat_lt(x, y)) {
		return EDOM;
	}

	/* x >= y */

	int err = -1;
	bignat tmp_diff = bignat_new_zero();
	uint32_t borrow = 0;
	uint64_t y_digit;
	uint32_t diff_digit;

	for (size_t i = 0; i < x.ndigits; i++) {
		y_digit = (uint64_t)(i < y.ndigits ? y.digits[i] : 0) +
			(uint64_t)borrow;
		borrow = x.digits[i] < y_digit;
		diff_digit = ((uint64_t)borrow << 32) +
			(uint64_t)x.digits[i] - y_digit;

		err = dgtvec_push(&tmp_diff, diff_digit);
		if (err != 0) {
			bignat_del(tmp_diff);
			return err;
		}
	}

	bignat_norm(&tmp_diff);

	*diff = tmp_diff;
	return 0;
}

static int
bignat_from_prod_digit(bignat *nat, uint64_t prod_digit, size_t start)
{
	int err = -1;
	bignat tmp_nat = bignat_new_zero();

	if (prod_digit == 0) {
		*nat = tmp_nat;
		return 0;
	}

	for (size_t i = 0; i < start; i++) {
		err = dgtvec_push(&tmp_nat, 0);
		if (err != 0) {
			goto fail;
		}
	}

	uint32_t low = prod_digit & ~(uint32_t)0;
	uint32_t high = prod_digit >> 32;

	err = dgtvec_push(&tmp_nat, low);
	if (err != 0) {
		goto fail;
	}

	if (high != 0) {
		err = dgtvec_push(&tmp_nat, high);
		if (err != 0) {
			goto fail;
		}
	}

	*nat = tmp_nat;
	return 0;

fail:
	bignat_del(tmp_nat);
	return err;
}

int
bignat_mul(bignat *prod, bignat x, bignat y)
{
	int err = -1;
	bignat tmp_prod = bignat_new_zero();

	for (size_t ix = 0; ix < x.ndigits; ix++) {
		for (size_t iy = 0; iy < y.ndigits; iy++) {
			uint64_t prod_digit = (uint64_t)x.digits[ix] *
				(uint64_t)y.digits[iy];

			bignat p;
			err = bignat_from_prod_digit(&p, prod_digit, ix + iy);
			if (err != 0) {
				bignat_del(tmp_prod);
				return err;
			}

			bignat new_tmp_prod;
			err = bignat_add(&new_tmp_prod, tmp_prod, p);
			if (err != 0) {
				bignat_del(p);
				bignat_del(tmp_prod);
				return err;
			}

			bignat_del(tmp_prod);
			bignat_del(p);
			tmp_prod = new_tmp_prod;
		}
	}

	*prod = tmp_prod;
	return 0;
}

/* TODO: 多倍長 */
int
bignat_div(bignat *quot, bignat x, bignat y)
{
	if (y.ndigits == 0) {
		return EDOM;
	}

	return bignat_init(quot, (uint32_t[]){x.digits[0] / y.digits[0]}, 1);
}
