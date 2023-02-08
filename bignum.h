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

int dgtvec_init(dgtvec *v, uint32_t *digits, size_t ndigits);
dgtvec dgtvec_new_empty(void);
void dgtvec_del(dgtvec v);
void dgtvec_dump(dgtvec v);
int dgtvec_push(dgtvec *v, uint32_t n);
uint32_t dgtvec_pop(dgtvec *v);

/* bignat */

/*
 * 先行0は常にすべて取り除かれる。特に、値0は要素数0として表現されるこ
 * とに注意。
 */
typedef dgtvec bignat;

int bignat_view(bignat *nat, uint32_t *digits, size_t ndigits);
int bignat_init(bignat *nat, uint32_t *digits, size_t ndigits);
int bignat_from_digit(bignat *nat, uint32_t n);
int bignat_copy(bignat *dst, bignat src);
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
int bignat_divmod(bignat *quot, bignat *rem, bignat x, bignat y);

int bignat_gcd(bignat *gcd, bignat x, bignat y);

/* bigint */

/*
 * signは-1か0か1となる。absが0であればsignは0を、absが0以外であれば
 * signも0以外を指定する必要がある。
 */
typedef struct bigint {
	int sign;
	bignat abs;
} bigint;

int bigint_init(bigint *int_, int sign, uint32_t *digits, size_t ndigits);
bignat bignat_new_zero(void);
int bigint_from_digit(bigint *int_, int32_t x);
int bigint_copy(bigint *dst, bigint src);
void bigint_del(bigint int_);

bool bigint_eq(bigint x, bigint y);
bool bigint_ne(bigint x, bigint y);
bool bigint_lt(bigint x, bigint y);
bool bigint_gt(bigint x, bigint y);
bool bigint_le(bigint x, bigint y);
bool bigint_ge(bigint x, bigint y);

int bigint_add(bigint *sum, bigint x, bigint y);
int bigint_sub(bigint *diff, bigint x, bigint y);
int bigint_mul(bigint *prod, bigint x, bigint y);
int bigint_divtrn(bigint *quot, bigint *rem, bigint x, bigint y);
int bigint_divflr(bigint *quot, bigint *rem, bigint x, bigint y);
int bigint_diveuc(bigint *quot, bigint *rem, bigint x, bigint y);

#endif /* BIGNUM_H */
