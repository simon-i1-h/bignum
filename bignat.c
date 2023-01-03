#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignat.h"

void *
xrealloc(void *ptr, size_t size)
{
	void *new_ptr = realloc(ptr, size);
	if (new_ptr == NULL) {
		fprintf(stderr, "realloc\n");
		exit(1);
	}
	return new_ptr;
}

static void
bignat_push(bignat *nat, uint32_t n)
{
	size_t new_ndigits = nat->ndigits + 1;
	nat->digits = xrealloc(nat->digits, new_ndigits);
	nat->ndigits = new_ndigits;
	nat->digits[new_ndigits - 1] = n;
}

bignat
bignat_new(uint32_t n)
{
	bignat nat = {
		.digits=xrealloc(NULL, sizeof(uint32_t)),
		.ndigits=1
	};
	nat.digits[0] = n;
	return nat;
}

void
bignat_del(bignat n)
{
	free(n.digits);
}

static int
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
	bignat tmp_sum = bignat_new(sum_digit & (uint64_t)0xffffffff);
	uint32_t carry = sum_digit >> 32;

	for (size_t i = 1; i < x.ndigits; i++) {
		if (i < y.ndigits) {
			sum_digit = (uint64_t)carry +
				(uint64_t)x.digits[i] + (uint64_t)y.digits[i];
			bignat_push(&tmp_sum,
				    sum_digit & (uint64_t)0xffffffff);
			carry = sum_digit >> 32;
		} else {
			sum_digit = (uint64_t)carry + (uint64_t)x.digits[i];
			bignat_push(&tmp_sum,
				    sum_digit & (uint64_t)0xffffffff);
			carry = sum_digit >> 32;
		}
	}

	if (carry != 0) {
		bignat_push(&tmp_sum, carry);
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

	bignat tmp_diff = (bignat){
		.digits=xrealloc(NULL, sizeof(bignat) * x.ndigits),
		.ndigits=x.ndigits
	};

	for (size_t i = x.ndigits - 1; i < x.ndigits; i--) {
		if (i < y.ndigits) {
			uint32_t borrow = 0;
			if (x.digits[i] < y.digits[i]) {
				tmp_diff.digits[i + 1]--;
				borrow = 1;
			}

			tmp_diff.digits[i] = ((uint64_t)borrow << 32) +
				(uint64_t)x.digits[i] - (uint64_t)y.digits[i];
		} else {
			tmp_diff.digits[i] = x.digits[i];
		}
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

/* TODO: 多倍長 */
int
bignat_mul(bignat *prod, bignat x, bignat y)
{
	*prod = bignat_new(x.digits[0] * y.digits[0]);
	return 0;
}

/* TODO: 多倍長 */
int
bignat_div(bignat *quot, bignat x, bignat y)
{
	if (y.digits[0] == 0) {
		return 1;
	}

	*quot = bignat_new(x.digits[0] / y.digits[0]);
	return 0;
}
