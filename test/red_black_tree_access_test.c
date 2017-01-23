#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */

static RedBlackTree tree;
static RedBlackTree tree_copy;

static inline void
verify_empty_tree(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");
}

static inline void
verify_unmodified_tree(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_congruent(&tree_copy,
							  &tree),
				 "TREE MODIFIED FROM ORIGINAL STATE");
}


void
setUp(void)
{
	int i;
	int key;
	bool status;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_put_new(&tree,
						(void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_tree_count(&tree),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_clone(&tree_copy,
						      &tree),
				 "OUT OF MEMORY");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_congruent(&tree,
							  &tree_copy),
				 "BAD CLONE");

	shuffle_keys();
}

void
tearDown(void)
{
	red_black_tree_destroy(&tree_copy);

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_find(void)
{
	int i;
	int key;
	bool status;

	status = red_black_tree_find(&tree,
				     (void *) -1);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED KEY");

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_find(&tree,
					     (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND KEY");
	}

	verify_unmodified_tree();
}


void
test_red_black_tree_fetch(void)
{
	int i;
	int key;
	void *fetched_key = NULL;
	void *fetched_key_initial;
	bool status;

	fetched_key_initial = fetched_key;

	status = red_black_tree_fetch(&tree,
				      (void *) -1,
				      &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCHED KEY VALUE CHANGED");

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_fetch(&tree,
					      (void *) (intptr_t) key,
					      &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      (int) (intptr_t) fetched_key,
					      "FETCHED KEY DOES NOT MATCH");
	}

	verify_unmodified_tree();
}


void
test_red_black_tree_find_min(void)
{
	int i;
	void *fetched_key;
	void *fetched_key_initial;
	bool status;

	for (i = 0; i < KEYS_COUNT; ++i) {
		status = red_black_tree_fetch_min(&tree,
						  &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND MIN KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) fetched_key,
					      "FETCHED MIN KEY DOES NOT MATCH");

		red_black_tree_drop_min(&tree);
	}

	verify_empty_tree();

	fetched_key_initial = fetched_key;

	status = red_black_tree_fetch_min(&tree,
					  &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED MIN KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCH KEY VALUE CHANGED");
}
