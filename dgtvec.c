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

static void *
xrealloc(void *ptr, size_t size)
{
	void *new_ptr = realloc(ptr, size);
	if (new_ptr == NULL) {
		fprintf(stderr, "realloc\n");
		exit(1);
	}
	return new_ptr;
}

dgtvec
dgtvec_new(uint32_t *digits, size_t ndigits)
{
	size_t cap = roundup_pow2(ndigits);

	dgtvec v = {
		.digits=NULL,
		.ndigits=ndigits,
		.cap=cap
	};

	if (ndigits == 0) {
		return v;
	}

	if (cap == 0) {
		fprintf(stderr, "dgtvec_new: ENOMEM\n");
		exit(1);
	}

	v.digits = xrealloc(NULL, sizeof(*digits) * cap);
	for (size_t i = 0; i < ndigits; i++) {
		v.digits[i] = digits[i];
	}

	return v;
}

void
dgtvec_del(dgtvec v)
{
	free(v.digits);
}

void
dgtvec_push(dgtvec *v, uint32_t n)
{
	if (v->cap == v->ndigits) {
		size_t cap = (v->cap + !v->cap) << 1;
		if (cap == 0) {
			fprintf(stderr, "dgtvec_push: ENOMEM\n");
			exit(1);
		}

		v->digits = xrealloc(v->digits, sizeof(*v->digits) * cap);
		v->cap = cap;
	}

	v->digits[v->ndigits++] = n;
}

uint32_t
dgtvec_pop(dgtvec *v)
{
	return v->digits[--v->ndigits];
}
