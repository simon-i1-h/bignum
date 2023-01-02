#ifndef BIGNUM_BIGNAT_H
#define BIGNUM_BIGNAT_H

#include <stdbool.h>

typedef struct bignat {
	unsigned int digit;
} bignat;

bignat bignat_new(unsigned int n);
void bignat_del(bignat n);

int bignat_eq(bool *dst, bignat x, bignat y);
int bignat_ne(bool *dst, bignat x, bignat y);

int bignat_add(bignat *sum, bignat x, bignat y);
int bignat_sub(bignat *diff, bignat x, bignat y);
int bignat_mul(bignat *prod, bignat x, bignat y);
int bignat_div(bignat *quot, bignat x, bignat y);

#endif /* BIGNUM_BIGNAT_H */
