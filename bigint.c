#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bignum.h"

bigint
bigint_new(int sign, uint32_t abs)
{
	if (abs == 0) {
		if (sign != 0) {
			fprintf(stderr, "invalid sign\n");
			exit(1);
		}
	} else {
		if (sign == 0) {
			fprintf(stderr, "invalid sign\n");
			exit(1);
		}
	}

	return (bigint){
		.sign=sign,
		.abs=bignat_new(abs)
	};
}

void
bigint_del(bigint int_)
{
	bignat_del(int_.abs);
}
