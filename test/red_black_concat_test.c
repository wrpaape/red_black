#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_concat.h"  /* red_black_concat */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;
static bool key_set[KEYS_COUNT];

void
setUp(void)
{
	int i;
	void *key;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		TEST_ASSERT_TRUE_MESSAGE(red_black_tree_put_new(&tree,
								key),
					 "OUT OF MEMORY");
	}

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_tree_count(&tree),
				       "UNEXPECTED COUNT");
}

void
tearDown(void)
{
	red_black_tree_destroy(&tree);
}


void
test_red_black_concat(void)
{
	int i;
	int key;
	bool *restrict key_set_ptr;
	struct RedBlackNode *restrict node;

	(void) red_black_concat(tree.root,
				&node);

	while (node != NULL) {
		key = (int) (intptr_t) node->key;

		key_set_ptr = &key_set[key];

		TEST_ASSERT_FALSE_MESSAGE(*key_set_ptr,
					  "LOOP DETECTED");

		*key_set_ptr = true;

		node = node->left;
	}

	for (i = 0; i < KEYS_COUNT; ++i)
		TEST_ASSERT_TRUE(key_set[i]);
}
