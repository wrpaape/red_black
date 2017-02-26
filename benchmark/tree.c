#include "red_black_tree.h"
#include <stddef.h>

static int keys[KEYS_COUNT];

int
compare_ints(const void *key1,
	     const void *key2)
{
	const int int1 = (int) (intptr_t) key1;
	const int int2 = (int) (intptr_t) key2;

	if (int1 < int2)
		return -1;

	if (int1 > int2)
		return 1;

	return 0;
}

int
main(void)
{
	RedBlackTree tree;
	int *restrict key;
	int i;

	red_black_tree_init(&tree,
			    &compare_ints);

	key = &keys[0];
	i   = 0;

	do {
		*key = i;

		(void) red_black_tree_add(&tree,
					  (void *) (intptr_t) i);
		++key;
		++i;
	} while (i < KEYS_COUNT);

	return 0;
}
