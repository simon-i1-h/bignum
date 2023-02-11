#include <errno.h>
#include <stdint.h>

#include "bignum.h"

int
bigrat_norm(bigrat *rat)
{
	if (rat->deno.sign == -1) {
		rat->nume.sign *= -1;
		rat->deno.sign = 1;
	}

	int err;
	bignat gcd;
	err = bignat_gcd(&gcd, rat->nume.abs, rat->deno.abs);
	if (err != 0) {
		return err;
	}

	bignat one;
	(void)bignat_view(&one, (uint32_t[]){1}, 1);
	if (bignat_eq(gcd, one)) {
		bignat_del(gcd);
		return 0;
	}

	bigint int_gcd;
	(void)bigint_view(&int_gcd, 1, gcd.digits, gcd.ndigits);

	bigint new_nume = bigint_new_zero();
	bigint new_deno = bigint_new_zero();
	bigint tmp_rem = bigint_new_zero();

	err = bigint_divtrn(&new_nume, &tmp_rem, rat->nume, int_gcd);
	if (err != 0) {
		goto fail;
	}
	bigint_del(tmp_rem);
	tmp_rem = bigint_new_zero();

	err = bigint_divtrn(&new_deno, &tmp_rem, rat->deno, int_gcd);
	if (err != 0) {
		goto fail;
	}
	bigint_del(tmp_rem);
	tmp_rem = bigint_new_zero();

	bigint_del(rat->nume);
	rat->nume = new_nume;
	bigint_del(rat->deno);
	rat->deno = new_deno;
	bignat_del(gcd);
	return 0;

fail:
	bignat_del(gcd);
	bigint_del(new_nume);
	bigint_del(new_deno);
	bigint_del(tmp_rem);
	return err;
}

int
bigrat_init(bigrat *rat,
	    int nume_sign, uint32_t *nume_digits, size_t nume_ndigits,
	    int deno_sign, uint32_t *deno_digits, size_t deno_ndigits)
{
	if (deno_sign == 0 && deno_ndigits == 0) {
		return EINVAL;
	}

	int err;
	bigint nume;
	err = bigint_init(&nume, nume_sign, nume_digits, nume_ndigits);
	if (err != 0) {
		return err;
	}

	bigint deno;
	err = bigint_init(&deno, deno_sign, deno_digits, deno_ndigits);
	if (err != 0) {
		bigint_del(nume);
		return err;
	}

	bigrat tmp_rat = (bigrat){
		.nume=nume,
		.deno=deno
	};
	err = bigrat_norm(&tmp_rat);
	if (err != 0) {
		bigrat_del(tmp_rat);
		return err;
	}

	*rat = tmp_rat;
	return 0;
}

int
bigrat_from_digit(bigrat *rat, int32_t nume, int32_t deno)
{
	int err;
	bigint int_nume;
	err = bigint_from_digit(&int_nume, nume);
	if (err != 0) {
		return err;
	}

	bigint int_deno;
	err = bigint_from_digit(&int_deno, deno);
	if (err != 0) {
		bigint_del(int_nume);
		return err;
	}

	bigrat tmp_rat = (bigrat){
		.nume=int_nume,
		.deno=int_deno
	};

	err = bigrat_norm(&tmp_rat);
	if (err != 0) {
		bigrat_del(tmp_rat);
	}

	*rat = tmp_rat;
	return 0;
}

void
bigrat_del(bigrat rat)
{
	bigint_del(rat.nume);
	bigint_del(rat.deno);
}

int
bigrat_cmp(int *cmp, bigrat x, bigrat y)
{
	if (bigint_eq(x.deno, y.deno)) {
		*cmp = bigint_cmp(x.nume, y.nume);
		return 0;
	}

	int err;
	bigint norm_x;
	err = bigint_mul(&norm_x, x.nume, y.deno);
	if (err != 0) {
		return err;
	}

	bigint norm_y;
	err = bigint_mul(&norm_y, y.nume, x.deno);
	if (err != 0) {
		bigint_del(norm_x);
		return err;
	}

	int tmp_cmp = bigint_cmp(norm_x, norm_y);
	bigint_del(norm_x);
	bigint_del(norm_y);
	*cmp = tmp_cmp;
	return 0;
}

int
bigrat_eq(bool *eq, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*eq = cmp == 0;
	return 0;
}

int
bigrat_ne(bool *ne, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*ne = cmp != 0;
	return 0;
}

int
bigrat_lt(bool *lt, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*lt = cmp < 0;
	return 0;
}

int
bigrat_gt(bool *gt, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*gt = cmp > 0;
	return 0;
}

int
bigrat_le(bool *le, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*le = cmp <= 0;
	return 0;
}

int
bigrat_ge(bool *ge, bigrat x, bigrat y)
{
	int err;
	int cmp;
	err = bigrat_cmp(&cmp, x, y);
	if (err != 0) {
		return err;
	}

	*ge = cmp >= 0;
	return 0;
}
