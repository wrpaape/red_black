#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;

static inline void
verify_empty_tree(void)
{

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");
}

void
setUp(void)
{
	int i;
	void *key;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	verify_empty_tree();

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

	shuffle_keys();
}

void
tearDown(void)
{
	verify_empty_tree();

	red_black_tree_destroy(&tree);
}

void
test_red_black_tree_delete(void)
{
	int i;
	int status;
	void *key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_delete(&tree,
					       key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "DELETED KEY TWICE (0)");
	}
}

void
test_red_black_tree_delete_min(void)
{
	int i;
	int status;
	void *key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED MIN KEY");

		status = red_black_tree_delete_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "DELETED KEY TWICE (0)");
	}

	verify_empty_tree();

	status = red_black_tree_delete_min(&tree);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "DELETED FROM EMTPY TREE (1)");
}

void
test_red_black_tree_delete_max(void)
{
	int i;
	int status;
	void *key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED MAX KEY");

		status = red_black_tree_delete_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "DELETED KEY TWICE (0)");
	}

	verify_empty_tree();

	status = red_black_tree_delete_max(&tree);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "DELETED FROM EMTPY TREE (1)");
}


void
test_red_black_tree_remove(void)
{
	int i;
	int status;
	void *key;
	void *old_key;
	void *old_key_initial;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		status = red_black_tree_remove(&tree,
					       key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(key,
					      old_key,
					      "OLD KEY DOES NOT MATCH");
	}

	verify_empty_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (void *) (intptr_t) keys[i];

		old_key_initial = old_key;

		status = red_black_tree_remove(&tree,
					       key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-REMOVED USED KEY (1)");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(old_key_initial,
					      old_key,
					      "VALUE OF OLD KEY CHANGED");
	}
}

void
test_red_black_tree_remove_min(void)
{
	int i;
	int status;
	void *key;
	void *key_initial;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED MIN KEY");

		status = red_black_tree_remove_min(&tree,
						   &key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED REMOVED KEY");
	}

	verify_empty_tree();

	key_initial = key;

	status = red_black_tree_remove_min(&tree,
					   &key);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "REMOVED FROM EMTPY TREE (1)");

	TEST_ASSERT_EQUAL_PTR_MESSAGE(key_initial,
				      key,
				      "VALUE OF KEY CHANGED");
}

void
test_red_black_tree_remove_max(void)
{
	int i;
	int status;
	void *key;
	void *key_initial;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED MAX KEY");

		status = red_black_tree_remove_max(&tree,
						   &key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) key,
					      "UNEXPECTED REMOVED KEY");
	}

	verify_empty_tree();

	key_initial = key;

	status = red_black_tree_remove_max(&tree,
					   &key);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "REMOVED FROM EMTPY TREE (1)");

	TEST_ASSERT_EQUAL_PTR_MESSAGE(key_initial,
				      key,
				      "VALUE OF KEY CHANGED");
}
