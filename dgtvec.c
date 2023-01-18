/* for reallocarray */
#define _DEFAULT_SOURCE

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

/* 切り上げで桁が溢れた場合は0を返す。 */
static size_t
roundup_pow2(size_t n)
{
	n--;
	for (size_t i = 1; i < SIZE_WIDTH; i <<= 1) {
		n |= n >> i;
	}
	n++;
	return n;
}

int
dgtvec_init(dgtvec *v, uint32_t *digits, size_t ndigits)
{
	dgtvec tv = {
		.digits=NULL,
		.ndigits=ndigits,
		.cap=roundup_pow2(ndigits)
	};

	if (ndigits == 0) {
		*v = tv;
		return 0;
	}

	if (tv.cap == 0) {
		return ENOMEM;
	}

	tv.digits = reallocarray(NULL, tv.cap, sizeof(*tv.digits));
	if (tv.digits == NULL) {
		return ENOMEM;
	}

	for (size_t i = 0; i < tv.ndigits; i++) {
		tv.digits[i] = digits[i];
	}

	*v = tv;
	return 0;
}

dgtvec
dgtvec_new_empty(void)
{
	return (dgtvec){
		.digits=NULL,
		.ndigits=0,
		.cap=0
	};
}

/* TODO: 削除 */
dgtvec
dgtvec_new(uint32_t *digits, size_t ndigits)
{
	dgtvec v;
	if (dgtvec_init(&v, digits, ndigits) != 0) {
		fprintf(stderr, "dgtvec_new: ENOMEM\n");
		exit(1);
	}

	return v;
}

void
dgtvec_del(dgtvec v)
{
	free(v.digits);
}

void
dgtvec_dump(dgtvec v)
{
	printf("dump dgtvec---------\n");
	printf("digits: ");
	if (v.ndigits == 0) {
		printf("(none)");
	} else {
		printf("%d", v.digits[0]);
		for (size_t i = 1; i < v.ndigits; i++) {
			printf(", %d", v.digits[i]);
		}
	}
	printf("\n");
	printf("ndigits: %zu\n", v.ndigits);
	printf("cap: %zu\n", v.cap);
	printf("--------------------\n");
}


static int
dgtvec_mpush(dgtvec *v, uint32_t n)
{
	if (v->cap == v->ndigits) {
		size_t cap = (v->cap + !v->cap) << 1;
		if (cap == 0) {
			return ENOMEM;
		}

		void *digits;
		digits = reallocarray(v->digits, cap, sizeof(*v->digits));
		if (digits == NULL) {
			return ENOMEM;
		}

		v->digits = digits;
		v->cap = cap;
	}

	v->digits[v->ndigits++] = n;
	return 0;
}

/* TODO: エラーメッセージの出力とexitをしない */
void
dgtvec_push(dgtvec *v, uint32_t n)
{
	if (dgtvec_mpush(v, n) != 0) {
		fprintf(stderr, "dgtvec_push: ENOMEM\n");
		exit(1);
	}
}

uint32_t
dgtvec_pop(dgtvec *v)
{
	return v->digits[--v->ndigits];
}
