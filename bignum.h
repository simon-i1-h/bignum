#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* dgtvec */

/*
 * uint32_t型の値を要素する可変長配列。
 */
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
 * dgtvecに基づく、(0を含む)自然数型。dgtvecの最初の要素を最下位の桁と
 * して、より後方の要素ほど大きい桁を表す。2の32乗進数と考えると分かり
 * やすい。
 *
 * 先行0は常にすべて取り除かれる。特に、値0は要素数0として表現されるこ
 * とに注意。
 */
typedef dgtvec bignat;

int bignat_view(bignat *nat, uint32_t *digits, size_t ndigits);
int bignat_init(bignat *nat, uint32_t *digits, size_t ndigits);
bignat bignat_new_zero(void);
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
 * bignatに基づく整数型。bignatと符号変数で整数を表す。符号変数の符号
 * でbigintの符号を表す。
 *
 * signは-1か0か1となる。absが0であればsignは0を、absが0以外であれば
 * signも0以外を指定する必要がある。
 */
typedef struct bigint {
	int sign;
	bignat abs;
} bigint;

int bigint_view(bigint *int_, int sign, uint32_t *digits, size_t ndigits);
int bigint_init(bigint *int_, int sign, uint32_t *digits, size_t ndigits);
bigint bigint_new_zero(void);
int bigint_from_digit(bigint *int_, int32_t x);
int bigint_copy(bigint *dst, bigint src);
void bigint_del(bigint int_);

int bigint_cmp(bigint x, bigint y);
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

/* bigrat */

/*
 * bigintに基づく有理数型。2つのbigintをそれぞれ分子と分母として有理数
 * を表す。
 *
 * denoは0にできない。
 *
 * numeとdenoは互いに素になるように正規化される。また、denoは正の整数
 * になるように正規化される。これらの正規化は必須ではないが、bigratの
 * APIは常に正規化されたbigrat型の値を返す。
 */
typedef struct bigrat {
	bigint nume;
	bigint deno;
} bigrat;

int bigrat_init(bigrat *rat,
		int nume_sign, uint32_t *nume_digits, size_t nume_ndigits,
		int deno_sign, uint32_t *deno_digits, size_t deno_ndigits);
int bigrat_from_digit(bigrat *rat, int32_t nume, int32_t deno);
int bigrat_copy(bigrat *dst, bigrat src);
void bigrat_del(bigrat rat);

int bigrat_cmp(int *cmp, bigrat x, bigrat y);
int bigrat_eq(bool *eq, bigrat x, bigrat y);
int bigrat_ne(bool *ne, bigrat x, bigrat y);
int bigrat_lt(bool *lt, bigrat x, bigrat y);
int bigrat_gt(bool *gt, bigrat x, bigrat y);
int bigrat_le(bool *le, bigrat x, bigrat y);
int bigrat_ge(bool *ge, bigrat x, bigrat y);

int bigrat_add(bigrat *sum, bigrat x, bigrat y);
int bigrat_sub(bigrat *diff, bigrat x, bigrat y);
int bigrat_mul(bigrat *prod, bigrat x, bigrat y);
int bigrat_div(bigrat *quot, bigrat x, bigrat y);

int bigrat_trn(bigrat *int_, bigrat *frac, bigrat rat);

#endif /* BIGNUM_H */
