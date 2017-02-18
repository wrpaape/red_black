#include "unity.h"          /* UNITY testing framework */
#include "red_black_tree.h" /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <stdint.h>	    /* intptr_t */


static RedBlackTree asc_tree;
static RedBlackTree desc_tree;


static inline void
verify_empty_tree(RedBlackTree *const restrict tree)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(tree),
				       "NON-EMPTY TREE");
}


static inline void
verify_full_tree(const RedBlackTree *const restrict tree)
{
	TEST_ASSERT_FALSE_MESSAGE(red_black_tree_empty(tree),
				  "EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_tree_count(tree),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(tree),
				 "INVALID TREE");
}


void
setUp(void)
{
	int i;
	int key;
	bool status;

	red_black_tree_init(&asc_tree,
			    &int_key_comparator);

	verify_empty_tree(&asc_tree);

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_add(&asc_tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	verify_full_tree(&asc_tree);

	red_black_tree_init(&desc_tree,
			    &int_key_comparator);

	verify_empty_tree(&desc_tree);

	while (--i >= 0) {
		key = keys[i];

		status = red_black_tree_add(&desc_tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	verify_full_tree(&desc_tree);
}

void
tearDown(void)
{
	red_black_tree_destroy(&asc_tree);
	red_black_tree_destroy(&desc_tree);
}


void
test_red_black_tree_compare(void)
{
	bool status;
	int i;
	int key;
	RedBlackTree *restrict tree_ptr;
	RedBlackTree positive_tree;

	/* compare ASC tree to DESC tree */
	status = red_black_tree_similar(&asc_tree,
					&desc_tree)
	      && red_black_tree_similar(&desc_tree,
					&asc_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ASC/DESC TREES NOT SIMILAR");

	status = red_black_tree_congruent(&asc_tree,
					  &desc_tree)
	      || red_black_tree_congruent(&desc_tree,
					  &asc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ASC/DESC TREES UNEXPECTEDLY CONGRUENT");

	red_black_tree_init(&positive_tree,
			    &int_key_comparator);

	verify_empty_tree(&positive_tree);

	/* delete ODD  keys from asc_tree
	 * delete EVEN keys from desc_tree
	 * add    ALL  keys to   positive_tree */

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_add(&positive_tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY")
		tree_ptr = (key & 1)
			 ? &asc_tree
			 : &desc_tree;

		red_black_tree_drop(tree_ptr,
				    (void *) (intptr_t) key);

	}

	/* asc_tree	 contains only EVEN keys
	 * desc_tree	 contains only ODD  keys
	 * positive_tree contains ALL       keys */

	/* compare EVEN tree to POSITIVE tree */
	status = red_black_tree_intersect(&positive_tree,
					  &asc_tree)
	      && red_black_tree_intersect(&asc_tree,
					  &positive_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "EVEN TREE DOES NOT INTERSECT WITH "
				 "POSITIVE TREE");

	status = red_black_tree_subset(&positive_tree,
				       &asc_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "EVEN TREE IS NOT A SUBSET OF POSITIVE TREE");

	status = red_black_tree_subset(&asc_tree,
				       &positive_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "POSITIVE TREE IS A SUBSET OF EVEN TREE");

	status = red_black_tree_add(&asc_tree,
				    (void *) (intptr_t) -2);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_tree_subset(&positive_tree,
				       &asc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN TREE WITH NEGATIVE KEY IS A SUBSET OF "
				  "POSITIVE TREE");

	/* compare ODD tree to POSITIVE tree */
	status = red_black_tree_intersect(&positive_tree,
					  &desc_tree)
	      && red_black_tree_intersect(&desc_tree,
					  &positive_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ODD TREE DOES NOT INTERSECT WITH "
				 "POSITIVE TREE");

	status = red_black_tree_subset(&positive_tree,
				       &desc_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ODD TREE IS NOT A SUBSET OF POSITIVE TREE");

	status = red_black_tree_subset(&desc_tree,
				       &positive_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "POSITIVE TREE IS A SUBSET OF ODD TREE");

	status = red_black_tree_add(&desc_tree,
				    (void *) (intptr_t) -1);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_tree_subset(&positive_tree,
				       &desc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ODD TREE WITH NEGATIVE KEY IS A SUBSET OF "
				  "POSITIVE TREE");

	red_black_tree_destroy(&positive_tree); /* done with positive tree */

	/* compare EVEN tree to ODD tree */
	status = red_black_tree_intersect(&asc_tree,
					  &desc_tree)
	      || red_black_tree_intersect(&desc_tree,
					  &asc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN TREE INTERSECTS WITH ODD TREE");

	status = red_black_tree_subset(&asc_tree,
				       &desc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN TREE IS A SUBSET OF ODD TREE");

	status = red_black_tree_subset(&asc_tree,
				       &desc_tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ODD TREE IS A SUBSET OF EVEN TREE");
}


static inline void
do_test_red_black_tree_clone(const RedBlackTree *const restrict other_tree)
{
	bool status;
	RedBlackTree tree;

	status = red_black_tree_clone(&tree,
				      other_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_tree_similar(&tree,
					other_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OTHER TREE AND CLONE NOT SIMILAR");

	status = red_black_tree_congruent(&tree,
					  other_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OTHER TREE AND CLONE NOT CONGRUENT");

	red_black_tree_destroy(&tree);
}

void
test_red_black_tree_clone(void)
{
	do_test_red_black_tree_clone(&asc_tree);
	do_test_red_black_tree_clone(&desc_tree);
}
