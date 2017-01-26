#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;

static inline void
verify_empty_tree(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_empty(&tree),
				 "NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");
}

void
setUp(void)
{
	int i;
	int key;
	bool status;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	verify_empty_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_add(&tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
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
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_delete(&tree,
					       (void *) (intptr_t) key);

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
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
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
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
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
	int key;
	void *old_key;
	void *old_key_initial;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_remove(&tree,
					       (void *) (intptr_t) key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      (int) (intptr_t) old_key,
					      "OLD KEY DOES NOT MATCH");
	}

	verify_empty_tree();

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		old_key_initial = old_key;

		status = red_black_tree_remove(&tree,
					       (void *) (intptr_t) key,
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-REMOVED USED KEY (1)");

		TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) old_key_initial,
					      (int) (intptr_t) old_key,
					      "VALUE OF OLD KEY CHANGED");
	}
}

void
test_red_black_tree_remove_min(void)
{
	int i;
	int status;
	int key;
	void *removed_key_initial;
	void *removed_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MIN KEY");

		status = red_black_tree_remove_min(&tree,
						   &removed_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) removed_key,
					      "UNEXPECTED REMOVED KEY");
	}

	verify_empty_tree();

	removed_key_initial = removed_key;

	status = red_black_tree_remove_min(&tree,
					   &removed_key);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "REMOVED FROM EMTPY TREE (1)");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) removed_key_initial,
				      (int) (intptr_t) removed_key,
				      "VALUE OF KEY CHANGED");
}

void
test_red_black_tree_remove_max(void)
{
	int i;
	int status;
	int key;
	void *removed_key_initial;
	void *removed_key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MAX KEY");

		status = red_black_tree_remove_max(&tree,
						   &removed_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) removed_key,
					      "UNEXPECTED REMOVED KEY");
	}

	verify_empty_tree();

	removed_key_initial = removed_key;

	status = red_black_tree_remove_max(&tree,
					   &removed_key);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      status,
				      "REMOVED FROM EMTPY TREE (1)");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) removed_key_initial,
				      (int) (intptr_t) removed_key,
				      "VALUE OF KEY CHANGED");
}


void
test_red_black_tree_drop(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		red_black_tree_drop(&tree,
				    (void *) (intptr_t) key);
	}
}

void
test_red_black_tree_drop_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MIN KEY");

		red_black_tree_drop_min(&tree);
	}
}

void
test_red_black_tree_drop_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MAX KEY");

		red_black_tree_drop_max(&tree);
	}
}


void
test_red_black_tree_pluck(void)
{
	int i;
	int key;
	int old_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		old_key = (int) (intptr_t)
			  red_black_tree_pluck(&tree,
					       (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      old_key,
					      "OLD KEY DOES NOT MATCH");
	}
}

void
test_red_black_tree_pluck_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MIN KEY");

		key = (int) (intptr_t) red_black_tree_pluck_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED PLUCKED KEY");
	}
}

void
test_red_black_tree_pluck_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MAX KEY");

		key = (int) (intptr_t) red_black_tree_pluck_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED PLUCKED KEY");
	}
}
