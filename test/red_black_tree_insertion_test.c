#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;

void
setUp(void)
{
	red_black_tree_init(&tree,
			    &int_key_comparator);

	shuffle_keys();
}


void
tearDown(void)
{
	red_black_tree_destroy(&tree);
}


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
					      "INSERTED USED KEY"
					      " OR OUT OF MEMORY");
	}

	verify_full_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_insert(&tree,
					       key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY"
					      " OR OUT OF MEMORY");
	}

	verify_full_tree();
}


void
test_red_black_tree_update(void)
{
	int i;
	int status;
	void *key;
	void *old_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_update(&tree,
					       key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY"
					      " OR OUT OF MEMORY");
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
					      "RE-INSERTED USED KEY"
					      " OR OUT OF MEMORY");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(key,
					      old_key,
					      "OLD KEY DOES NOT MATCH");
	}

	verify_full_tree();
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

	verify_full_tree();
}
