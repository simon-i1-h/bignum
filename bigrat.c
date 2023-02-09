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

void
bigrat_del(bigrat rat)
{
	bigint_del(rat.nume);
	bigint_del(rat.deno);
}
