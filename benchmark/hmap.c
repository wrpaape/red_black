#include "red_black_hmap.h"

static int keys[KEYS_COUNT];

int
main(void)
{
	RedBlackHMap map;
	int *restrict key;
	int i;

	(void) red_black_hmap_init(&map);

	key = &keys[0];
	i   = 0;

	do {
		*key = i;

		(void) red_black_hmap_add(&map,
					  key,
					  sizeof(*key));
		++key;
		++i;
	} while (i < KEYS_COUNT);

	return 0;
}
