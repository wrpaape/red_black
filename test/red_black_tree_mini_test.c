#include "red_black_tree.h"   /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"          /* int_key_comparator */
#include <stdio.h>            /* fprintf */
#include <stdlib.h>           /* exit */
#include <stdint.h>	      /* intptr_t */


static RedBlackTree tree;


static int keys[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

#define KEY_COUNT (sizeof(keys) / sizeof(keys[0]))

static bool key_set[KEY_COUNT];


#define TEST_FAILURE(FAILURE)						\
do {									\
	fprintf(stderr,							\
		FAILURE "\n");						\
	red_black_tree_destroy(&tree);					\
	exit(1);							\
} while (0)

int
main(void)
{
	bool *restrict key_set_ptr;
	int key;
	int prev_key;
	RedBlackTreeIterator iterator;
	int i;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	for (i = 0; i < KEY_COUNT; ++i) {
		if (!red_black_tree_put_new(&tree,
					    (const void *) (intptr_t) keys[i]))
			TEST_FAILURE("OUT OF MEMORY");

		if (!red_black_tree_verify(&tree))
			TEST_FAILURE("INVALID TREE");
	}

	if (red_black_tree_count(&tree) != KEY_COUNT)
			TEST_FAILURE("INCOMPLETE TREE -- count != KEY_COUNT");

	red_black_tree_asc_iterator_init(&iterator,
					 &tree);
	prev_key = -1;

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &key)) {
		if (prev_key > key)
			TEST_FAILURE("KEYS OUT OF ORDER");

		key_set_ptr = &key_set[key];

		if (*key_set_ptr)
			TEST_FAILURE("KEY REPEATED");

		*key_set_ptr = true;

		prev_key = key;
	}

	for (i = 0; i < KEY_COUNT; ++i)
		if (!key_set[i])
			TEST_FAILURE("INCOMPLETE TREE -- key unset");


	for (i = 0; i < KEY_COUNT; ++i) {
		red_black_tree_drop(&tree,
				    (const void *) (intptr_t) keys[i]);

		if (!red_black_tree_verify(&tree))
			TEST_FAILURE("INVALID TREE");
	}

	if (red_black_tree_count(&tree) != 0)
			TEST_FAILURE("INCOMPLETE TREE -- count != 0");

	puts("red_black_tree_mini_test PASSED");

	return 0;
}
