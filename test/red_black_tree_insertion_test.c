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
	void *key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_insert(&tree,
					       key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");
	}

	verify_full_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_insert(&tree,
					       key);

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
	void *key;
	void *old_key = 0;
	void *old_key_initial;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		old_key_initial = old_key;

		status = red_black_tree_update(&tree,
					       key,
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
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_update(&tree,
					       key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");

		TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) key,
					      (int) (intptr_t) old_key,
					      "OLD KEY DOES NOT MATCH");
	}
}


void
test_red_black_tree_put_new(void)
{
	int i;
	void *key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		TEST_ASSERT_TRUE_MESSAGE(red_black_tree_put_new(&tree,
								key),
					 "OUT OF MEMORY");
	}
}
