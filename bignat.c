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

static int
bignat_view(bignat *nat, uint32_t *digits, size_t ndigits)
{
	if (ndigits > 0 && digits[ndigits - 1] == 0) {
		return EINVAL;
	}

	*nat = (bignat){
		.digits=digits,
		.ndigits=ndigits,
		.cap=0
	};
	return 0;
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
bignat_accadd(bignat *dst, bignat src, size_t src_exp)
{
	int err = -1;
	uint32_t carry = 0;
	uint32_t dst_digit, src_digit, digit;
	uint64_t sum_digit;
	size_t ndigits = dst->ndigits > src.ndigits + src_exp
		? dst->ndigits
		: src.ndigits + src_exp;

	for (size_t i = 0; i < ndigits; i++) {
		dst_digit = i < dst->ndigits ? dst->digits[i] : 0;
		src_digit = i >= src_exp && i - src_exp < src.ndigits
			? src.digits[i - src_exp]
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

	err = bignat_accadd(&tmp_sum, y, 0);
	if (err != 0) {
		return err;
	}

	*sum = tmp_sum;
	return 0;
}

static int
bignat_accsub(bignat *dst, bignat src, size_t src_exp)
{
	if (dst->ndigits < src.ndigits + src_exp) {
		return EDOM;
	}

	if (dst->ndigits > src.ndigits + src_exp) {
		goto start;
	}

	for (size_t i = dst->ndigits - 1; i < dst->ndigits; i--) {
		uint32_t src_digit = i >= src_exp
			? src.digits[i - src_exp]
			: 0;

		if (dst->digits[i] < src_digit) {
			return EDOM;
		}

		if (dst->digits[i] > src_digit) {
			goto start;
		}
	}

	/* x >= y */

start:;
	uint32_t borrow = 0;
	uint64_t src_digit;
	uint32_t diff_digit;

	for (size_t i = 0; i < dst->ndigits; i++) {
		src_digit = i >= src_exp && i - src_exp < src.ndigits
			? src.digits[i - src_exp]
			: 0;
		src_digit += borrow;
		borrow = dst->digits[i] < src_digit;
		diff_digit = ((uint64_t)borrow << 32) +
			(uint64_t)dst->digits[i] - src_digit;
		dst->digits[i] = diff_digit;
	}

	bignat_norm(dst);
	return 0;
}

int
bignat_sub(bignat *diff, bignat x, bignat y)
{
	int err = -1;

	bignat tmp_diff;
	err = bignat_copy(&tmp_diff, x);
	if (err != 0) {
		return err;
	}

	err = bignat_accsub(&tmp_diff, y, 0);
	if (err != 0) {
		bignat_del(tmp_diff);
		return err;
	}

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
			size_t ndigits = countof(ds);
			while (ndigits > 0 && ds[ndigits - 1] == 0) {
				ndigits--;
			}
			bignat view;
			(void)bignat_view(&view, ds, ndigits);

			err = bignat_accadd(&tmp_prod, view, ix + iy);
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
bignat_divmod(bignat *quot, bignat *rem, bignat x, bignat y)
{
	if (y.ndigits == 0) {
		return EDOM;
	}

	bignat tmp_quot;
	bignat tmp_rem;
	int err = -1;

	err = bignat_from_digit(&tmp_quot, x.digits[0] / y.digits[0]);
	if (err != 0) {
		return err;
	}

	err = bignat_from_digit(&tmp_rem, x.digits[0] % y.digits[0]);
	if (err != 0) {
		bignat_del(tmp_quot);
		return err;
	}

	*quot = tmp_quot;
	*rem = tmp_rem;
	return 0;
}
