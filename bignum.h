#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* bignat */

typedef struct bignat {
	uint32_t *digits;
	size_t ndigits;
} bignat;

bignat bignat_new(uint32_t n);
void bignat_del(bignat n);

int bignat_eq(bool *dst, bignat x, bignat y);
int bignat_ne(bool *dst, bignat x, bignat y);
int bignat_lt(bool *dst, bignat x, bignat y);
int bignat_gt(bool *dst, bignat x, bignat y);
int bignat_le(bool *dst, bignat x, bignat y);
int bignat_ge(bool *dst, bignat x, bignat y);

int bignat_add(bignat *sum, bignat x, bignat y);
int bignat_sub(bignat *diff, bignat x, bignat y);
int bignat_mul(bignat *prod, bignat x, bignat y);
int bignat_div(bignat *quot, bignat x, bignat y);

#endif /* BIGNUM_H */
