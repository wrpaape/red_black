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
	unsigned long rand;

	const unsigned long span      = (unsigned long) (upto + 1);
	const unsigned long threshold = -span % span;

	do {
		rand = (unsigned long) random();
	} while (rand < threshold);

	return (unsigned int) (rand % span);
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
	srandom((unsigned) time(NULL));
}

void
shuffle_keys(void)
{
	int i_old;
	int i_new;

	ENTER("shuffle_keys");

	for (i_old = 0; i_old < I_LAST; ++i_old) {
		i_new = i_old + random_upto(I_LAST - i_old);

		swap(&keys[i_old],
		     &keys[i_new]);
	}

	RETURN("shuffle_keys");
}
