#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;

static inline void
verify_full_tree(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_tree_count(&tree),
				       "UNEXPECTED COUNT");
}

void
setUp(void)
{
	red_black_tree_init(&tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");

	shuffle_keys();
}


void
tearDown(void)
{
	verify_full_tree();

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_insert(void)
{
	int i;
	int status;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_insert(&tree,
					       (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");
	}

	verify_full_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_insert(&tree,
					       (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");
	}
}


void
test_red_black_tree_update(void)
{
	int i;
	int status;
	int key;
	void *old_key = NULL;
	void *old_key_initial;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		old_key_initial = old_key;

		status = red_black_tree_update(&tree,
					       (void *) (intptr_t) key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");

		TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) old_key_initial,
					      (int) (intptr_t) old_key,
					      "VALUE OF OLD KEY CHANGED");
	}

	verify_full_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_update(&tree,
					       (void *) (intptr_t) key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      (int) (intptr_t) old_key,
					      "OLD KEY DOES NOT MATCH");
	}
}


void
test_red_black_tree_put(void)
{
	int i;
	int key;
	bool status;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_put(&tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}
}
