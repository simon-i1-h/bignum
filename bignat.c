#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

#define countof(a) (sizeof(a) / sizeof((a)[0]))

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

int
bignat_copy(bignat *dst, bignat src)
{
	return bignat_init(dst, src.digits, src.ndigits);
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

/* 処理が失敗した場合、dstを解放する。 */
static int
bignat_acc(bignat *dst, uint32_t *src_digits, size_t src_ndigits,
	    size_t src_exp)
{
	int err = -1;
	if (src_ndigits > 0 && src_digits[src_ndigits - 1] == 0) {
		err = EINVAL;
		goto fail;
	}

	uint32_t carry = 0;
	uint32_t dst_digit, src_digit, digit;
	uint64_t sum_digit;
	size_t ndigits = dst->ndigits > src_ndigits
		? dst->ndigits
		: src_ndigits;

	for (size_t i = src_exp; i < ndigits; i++) {
		dst_digit = i < dst->ndigits ? dst->digits[i] : 0;
		src_digit = i - src_exp < src_ndigits
			? src_digits[i - src_exp]
			: 0;
		sum_digit = (uint64_t)dst_digit + (uint64_t)src_digit +
			(uint64_t)carry;
		digit = sum_digit & ~(uint32_t)0;
		carry = sum_digit >> 32;

		if (i < dst->ndigits) {
			dst->digits[i] = digit;
		} else {
			err = dgtvec_push(dst, digit);
			if (err != 0) {
				goto fail;
			}
		}
	}

	if (carry != 0) {
		err = dgtvec_push(dst, carry);
		if (err != 0) {
			goto fail;
		}
	}

	return 0;

fail:
	bignat_del(*dst);
	return err;
}

int
bignat_add(bignat *sum, bignat x, bignat y)
{
	int err = -1;

	bignat tmp_sum;
	err = bignat_copy(&tmp_sum, x);
	if (err != 0) {
		return err;
	}

	err = bignat_acc(&tmp_sum, y.digits, y.ndigits, 0);
	if (err != 0) {
		return err;
	}

	*sum = tmp_sum;
	return 0;
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

int
bignat_mul(bignat *prod, bignat x, bignat y)
{
	int err = -1;
	bignat tmp_prod = bignat_new_zero();

	for (size_t ix = 0; ix < x.ndigits; ix++) {
		for (size_t iy = 0; iy < y.ndigits; iy++) {
			uint64_t prod_digit = (uint64_t)x.digits[ix] *
				(uint64_t)y.digits[iy];
			uint32_t ds[2] = {
				prod_digit & ~(uint32_t)0,
				prod_digit >> 32
			};

			uint32_t *digits = NULL;
			size_t ndigits = countof(ds);
			for (; ndigits > 0; ndigits--) {
				if (ds[ndigits - 1] != 0) {
					digits = &ds[ndigits - 1];
					break;
				}
			}

			err = bignat_acc(&tmp_prod, digits, ndigits, ix + iy);
			if (err != 0) {
				return err;
			}
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
