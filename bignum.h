#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* dgtvec */

typedef struct dgtvec {
	uint32_t *digits;
	size_t ndigits;
	size_t cap;
} dgtvec;

dgtvec dgtvec_new(uint32_t *digits, size_t ndigits);
void dgtvec_del(dgtvec v);
void dgtvec_dump(dgtvec v);
void dgtvec_push(dgtvec *v, uint32_t n);
uint32_t dgtvec_pop(dgtvec *v);

/* bignat */

/*
 * 先行0は常にすべて取り除かれる。特に、値0は要素数0として表現されるこ
 * とに注意。
 */
typedef dgtvec bignat;

bignat bignat_new(uint32_t *digits, size_t ndigits);
bignat bignat_from_digit(uint32_t n);
void bignat_del(bignat n);

int bignat_cmp(bignat x, bignat y);
bool bignat_eq(bignat x, bignat y);
bool bignat_ne(bignat x, bignat y);
bool bignat_lt(bignat x, bignat y);
bool bignat_gt(bignat x, bignat y);
bool bignat_le(bignat x, bignat y);
bool bignat_ge(bignat x, bignat y);

int bignat_add(bignat *sum, bignat x, bignat y);
int bignat_sub(bignat *diff, bignat x, bignat y);
int bignat_mul(bignat *prod, bignat x, bignat y);
int bignat_div(bignat *quot, bignat x, bignat y);

typedef struct bigint {
	int sign;
	bignat abs;
} bigint;

bigint bigint_new(int sign, uint32_t *digits, size_t ndigits);
bigint bigint_from_digit(int32_t x);
void bigint_del(bigint int_);

bool bigint_eq(bigint x, bigint y);
bool bigint_ne(bigint x, bigint y);

#endif /* BIGNUM_H */
