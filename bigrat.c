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

int
bigrat_copy(bigrat *dst, bigrat src)
{
	int err;
	bigint tmp_nume;
	err = bigint_copy(&tmp_nume, src.nume);
	if (err != 0) {
		return err;
	}

	bigint tmp_deno;
	err = bigint_copy(&tmp_deno, src.deno);
	if (err != 0) {
		bigint_del(tmp_nume);
		return err;
	}

	*dst = (bigrat){
		.nume=tmp_nume,
		.deno=tmp_deno
	};
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

int
bigrat_add(bigrat *sum, bigrat x, bigrat y)
{
	if (bigint_eq(x.deno, y.deno)) {
		int err;
		bigint nume_sum;
		err = bigint_add(&nume_sum, x.nume, y.nume);
		if (err != 0) {
			return err;
		}

		bigint deno_sum;
		err = bigint_copy(&deno_sum, x.deno);
		if (err != 0) {
			bigint_del(nume_sum);
			return err;
		}

		bigrat tmp_sum = (bigrat){
			.nume=nume_sum,
			.deno=deno_sum
		};

		err = bigrat_norm(&tmp_sum);
		if (err != 0) {
			bigrat_del(tmp_sum);
			return err;
		}

		*sum = tmp_sum;
		return 0;
	}

	int err;
	bigint denorm_nume_x = bigint_new_zero();
	bigint denorm_nume_y = bigint_new_zero();
	bigint nume_sum = bigint_new_zero();
	bigint deno_sum = bigint_new_zero();

	err = bigint_mul(&denorm_nume_x, x.nume, y.deno);
	if (err != 0) {
		goto fail;
	}

	err = bigint_mul(&denorm_nume_y, y.nume, x.deno);
	if (err != 0) {
		goto fail;
	}

	err = bigint_add(&nume_sum, denorm_nume_x, denorm_nume_y);
	if (err != 0) {
		goto fail;
	}

	err = bigint_mul(&deno_sum, x.deno, y.deno);
	if (err != 0) {
		goto fail;
	}

	bigrat tmp_sum = (bigrat){
		.nume=nume_sum,
		.deno=deno_sum
	};
	err = bigrat_norm(&tmp_sum);
	if (err != 0) {
		bigint_del(denorm_nume_x);
		bigint_del(denorm_nume_y);
		bigrat_del(tmp_sum);
		return err;
	}

	bigint_del(denorm_nume_x);
	bigint_del(denorm_nume_y);
	*sum = tmp_sum;
	return 0;

fail:
	bigint_del(denorm_nume_x);
	bigint_del(denorm_nume_y);
	bigint_del(nume_sum);
	bigint_del(deno_sum);
	return err;
}

int
bigrat_sub(bigrat *diff, bigrat x, bigrat y)
{
	y.nume.sign *= -1;
	return bigrat_add(diff, x, y);
}

int
bigrat_mul(bigrat *prod, bigrat x, bigrat y)
{
	int err;
	bigint nume_prod = bigint_new_zero();
	bigint deno_prod = bigint_new_zero();

	err = bigint_mul(&nume_prod, x.nume, y.nume);
	if (err != 0) {
		goto fail;
	}

	err = bigint_mul(&deno_prod, x.deno, y.deno);
	if (err != 0) {
		goto fail;
	}

	bigrat tmp_prod = (bigrat){
		.nume=nume_prod,
		.deno=deno_prod
	};
	err = bigrat_norm(&tmp_prod);
	if (err != 0) {
		bigrat_del(tmp_prod);
		return err;
	}

	*prod = tmp_prod;
	return 0;

fail:
	bigint_del(nume_prod);
	bigint_del(deno_prod);
	return err;
}

int
bigrat_div(bigrat *quot, bigrat x, bigrat y)
{
	if (y.nume.sign == 0) {
		return EDOM;
	}

	bigrat inv_y = (bigrat){
		.nume=y.deno,
		.deno=y.nume
	};

	return bigrat_mul(quot, x, inv_y);
}

int
bigrat_trn(bigrat *int_, bigrat *frac, bigrat rat)
{
	int err;
	bigint quot = bigint_new_zero();
	bigint rem = bigint_new_zero();

	err = bigint_divtrn(&quot, &rem, rat.nume, rat.deno);
	if (err != 0) {
		goto fail;
	}

	bigrat tmp_frac;
	err = bigrat_init(&tmp_frac,
			  rem.sign, rem.abs.digits, rem.abs.ndigits,
			  rat.deno.sign, rat.deno.abs.digits, rat.deno.abs.ndigits);
	if (err != 0) {
		goto fail;
	}

	bigrat tmp_int;
	err = bigrat_sub(&tmp_int, rat, tmp_frac);
	if (err != 0) {
		bigrat_del(tmp_frac);
		goto fail;
	}

	bigint_del(quot);
	bigint_del(rem);
	*int_ = tmp_int;
	*frac = tmp_frac;
	return 0;

fail:
	bigint_del(quot);
	bigint_del(rem);
	return err;
}
