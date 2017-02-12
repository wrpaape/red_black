#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */



static RedBlackTree fizz_tree;
static RedBlackTree buzz_tree;
static RedBlackTree fizz_buzz_tree;


static inline void
init_int_tree(RedBlackTree *const restrict tree)
{
	red_black_tree_init(tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(tree),
				       "NON-EMPTY TREE");
}

void
setUp(void)
{
	int i;
	int key;
	bool status;

	init_int_tree(&fizz_tree);
	init_int_tree(&buzz_tree);
	init_int_tree(&fizz_buzz_tree);

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		if ((key % 15) == 0)
			status = red_black_tree_add(&fizz_buzz_tree,
						    (void *) (intptr_t) key);
		else if ((key % 3) == 0)
			status = red_black_tree_add(&fizz_tree,
						    (void *) (intptr_t) key);
		else if ((key % 5) == 0)
			status = red_black_tree_add(&buzz_tree,
						    (void *) (intptr_t) key);
		else
			continue;

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}
}
