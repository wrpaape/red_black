#include "unity.h"          /* UNITY testing framework */
#include "red_black_tree.h" /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <stdint.h>	    /* intptr_t */

static RedBlackTree tree;
static RedBlackTree tree_copy;

void
setUp(void)
{
	int i;
	int key;
	bool status;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_empty(&tree),
				 "NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(&tree),
				       "NON-EMPTY TREE");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_put(&tree,
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
}

void
tearDown(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_congruent(&tree_copy,
							  &tree),
				 "TREE MODIFIED FROM ORIGINAL STATE");

	red_black_tree_destroy(&tree_copy);

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_asc_etor(void)
{
	int i;
	int j;
	void *key;
	bool status;
	RedBlackTreeEtor etor;

	red_black_tree_asc_etor_init(&etor,
					 &tree);

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < KEYS_COUNT; ++j) {
			status = red_black_tree_etor_next(&etor,
							  &key);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "ASC ITERATOR SKIPPED KEYS");

			TEST_ASSERT_EQUAL_INT_MESSAGE(j,
						      (int) (intptr_t) key,
						      "UNEXPECTED NEXT KEY");
		}

		status = red_black_tree_etor_next(&etor,
						  &key);

		TEST_ASSERT_FALSE_MESSAGE(status,
					  "KEYS REMAIN AFTER ASC ITERATION");

		red_black_tree_etor_reset(&etor,
					      &tree);
	}
}


void
test_red_black_tree_desc_etor(void)
{
	int i;
	int j;
	void *key;
	bool status;
	RedBlackTreeEtor etor;

	red_black_tree_desc_etor_init(&etor,
				      &tree);

	for (i = 0; i < 2; ++i) {
		for (j = KEYS_COUNT - 1; j >= 0; --j) {
			status = red_black_tree_etor_next(&etor,
							  &key);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "DESC ITERATOR SKIPPED KEYS");

			TEST_ASSERT_EQUAL_INT_MESSAGE(j,
						      (int) (intptr_t) key,
						      "UNEXPECTED NEXT KEY");
		}

		status = red_black_tree_etor_next(&etor,
						  &key);

		TEST_ASSERT_FALSE_MESSAGE(status,
					  "KEYS REMAIN AFTER DESC ITERATION");

		red_black_tree_etor_reset(&etor,
					  &tree);
	}
}
