#include "red_black_test.h" /* KEYS_COUNT */
#include <time.h>	    /* time */

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
int keys[KEYS_COUNT];
const int *const restrict keys_until = &keys[KEYS_COUNT];

#define I_LAST (KEYS_COUNT - 1)


unsigned int
random_upto(const unsigned int upto)
{
	unsigned int random;

	/* range_length: length of inclusive range 0 ... upto
	 * valid_limit:  largest raw random unsigned long which will not map
	 * uniformly when MOD'd by 'range_length' (i.e. favor some values in range
	 * 0 ... upto more than others). */
	const unsigned int range_length = upto + 1;
	const unsigned int valid_limit  = RAND_MAX - (RAND_MAX % range_length);

	do {
		random = (unsigned int) rand();
	} while (random > valid_limit);

	return random % range_length;
}

static inline void
swap(int *const restrict x,
     int *const restrict y)
{
	const int tmp = *x;
	*x = *y;
	*y = tmp;
}

void
init_keys(void)
{
	int i;

	i = 0;

	do {
		keys[i] = i;
		++i;
	} while (i < KEYS_COUNT);
}

void
seed_random(void)
{
	srand((unsigned) time(NULL));
}

void
shuffle_keys(void)
{
	int i_old;
	int i_new;

	for (i_old = 0; i_old < I_LAST; ++i_old) {
		i_new = i_old + random_upto(I_LAST - i_old);

		swap(&keys[i_old],
		     &keys[i_new]);
	}
}
