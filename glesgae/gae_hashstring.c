#include "gae.h"

gae_hashstring gae_hashstring_calculate(const char* string) {
	int c;
	gae_hashstring hash = 0U;

	while ((c = *string++))
		hash = ((hash << 5) + hash) ^ c;

	return hash;
}

int gae_hashstring_compare(void* const A, void* const B) {
	gae_hashstring* a = (gae_hashstring*)A;
	gae_hashstring* b = (gae_hashstring*)B;

	return *a==*b;
}

