#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

bignat
bignat_from_digit(uint32_t n)
{
	return dgtvec_new(&n, 1);
}

void
bignat_del(bignat n)
{
	dgtvec_del(n);
}

int
bignat_cmp(int *dst, bignat x, bignat y)
{
	if (x.ndigits < y.ndigits) {
		*dst = -1;
		return 0;
	}

	if (x.ndigits > y.ndigits) {
		*dst = 1;
		return 0;
	}

	/* x.ndigits == y.ndigits */

	for (size_t i = x.ndigits - 1; i < x.ndigits; i--) {
		if (x.digits[i] < y.digits[i]) {
			*dst = -1;
			return 0;
		}

		if (x.digits[i] > y.digits[i]) {
			*dst = 1;
			return 0;
		}
	}

	*dst = 0;
	return 0;
}

int
bignat_eq(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp == 0;
	}
	return err;
}

int
bignat_ne(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp != 0;
	}
	return err;
}

int
bignat_lt(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp < 0;
	}
	return err;
}

int
bignat_gt(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp > 0;
	}
	return err;
}

int
bignat_le(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp <= 0;
	}
	return err;
}

int
bignat_ge(bool *dst, bignat x, bignat y)
{
	int cmp;
	int err = bignat_cmp(&cmp, x, y);
	if (err == 0) {
		*dst = cmp >= 0;
	}
	return err;
}

int
bignat_add(bignat *sum, bignat x, bignat y)
{
	if (x.ndigits < y.ndigits) {
		return bignat_add(sum, y, x);
	}

	/* x.ndigits >= y.ndigits */

	uint64_t sum_digit = (uint64_t)x.digits[0] + (uint64_t)y.digits[0];
	bignat tmp_sum = bignat_from_digit(sum_digit & (uint64_t)0xffffffff);
	uint32_t carry = sum_digit >> 32;

	for (size_t i = 1; i < x.ndigits; i++) {
		uint64_t y_digit = i < y.ndigits ? y.digits[i] : 0;
		sum_digit = (uint64_t)carry + (uint64_t)x.digits[i] + y_digit;
		dgtvec_push(&tmp_sum, sum_digit & (uint64_t)0xffffffff);
		carry = sum_digit >> 32;
	}

	if (carry != 0) {
		dgtvec_push(&tmp_sum, carry);
	}

	*sum = tmp_sum;
	return 0;
}

int
bignat_sub(bignat *diff, bignat x, bignat y)
{
	bool x_is_less;
	int err = bignat_lt(&x_is_less, x, y);
	if (err || x_is_less) {
		return 1;
	}

	/* x >= y */

	uint32_t borrow = x.digits[0] < y.digits[0];
	uint32_t diff_digit = ((uint64_t)borrow << 32) +
			(uint64_t)x.digits[0] - y.digits[0];
	bignat tmp_diff = bignat_from_digit(diff_digit);

	for (size_t i = 1; i < x.ndigits; i++) {
		uint64_t y_digit = (uint64_t)borrow +
			(uint64_t)(i < y.ndigits ? y.digits[i] : 0);
		borrow = x.digits[i] < y_digit;
		diff_digit = ((uint64_t)borrow << 32) +
			(uint64_t)x.digits[i] - y_digit;
		dgtvec_push(&tmp_diff, diff_digit);
	}

	/* 末尾0の削除 */
	for (size_t i = tmp_diff.ndigits - 1; i != 0; i--) {
		if (tmp_diff.digits[i] != 0) {
			break;
		}
		tmp_diff.ndigits--;
	}

	*diff = tmp_diff;
	return 0;
}

static bignat
bignat_from_prod_digit(uint64_t prod_digit, size_t start)
{
	uint32_t high = prod_digit >> 32;
	uint32_t low = prod_digit & 0xffffffff;

	bignat nat = dgtvec_new(NULL, 0);
	for (size_t i = 0; i < start; i++) {
		dgtvec_push(&nat, 0);
	}

	dgtvec_push(&nat, low);
	if (high != 0) {
		dgtvec_push(&nat, high);
	}

	/* TODO norm */
	return nat;
}

int
bignat_mul(bignat *prod, bignat x, bignat y)
{
	bignat tmp_prod = bignat_from_digit(0);

	for (size_t ix = 0; ix < x.ndigits; ix++) {
		for (size_t iy = 0; iy < y.ndigits; iy++) {
			uint64_t prod_digit = (uint64_t)x.digits[ix] *
				(uint64_t)y.digits[iy];
			bignat new_tmp_prod;
			bignat p = bignat_from_prod_digit(
				prod_digit, ix + iy);
			bignat_add(&new_tmp_prod, tmp_prod, p);
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
	if (y.digits[0] == 0) {
		return 1;
	}

	*quot = bignat_from_digit(x.digits[0] / y.digits[0]);
	return 0;
}
