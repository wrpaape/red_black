#include "unity.h"          /* UNITY testing framework */
#include "red_black_tree.h" /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <stdint.h>	    /* intptr_t */
#include <limits.h>	    /* INT_MIN|MAX */

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
test_red_black_tree_fetch_min(void)
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

		red_black_tree_subtract_min(&tree);
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


void
test_red_black_tree_fetch_max(void)
{
	int i;
	void *fetched_key;
	void *fetched_key_initial;
	bool status;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		status = red_black_tree_fetch_max(&tree,
						  &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND MAX KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) fetched_key,
					      "FETCHED MAX KEY DOES NOT MATCH");

		red_black_tree_subtract_max(&tree);
	}

	verify_empty_tree();

	fetched_key_initial = fetched_key;

	status = red_black_tree_fetch_max(&tree,
					  &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED MAX KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCH KEY VALUE CHANGED");
}

void
test_red_black_tree_replace(void)
{
	int key;
	void *replaced_key = NULL;
	void *replaced_key_initial;
	bool status;

	replaced_key_initial = replaced_key;

	status = red_black_tree_replace(&tree,
					(void *) -1,
					&replaced_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "REPLACED UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) replaced_key_initial,
				      (int) (intptr_t) replaced_key,
				      "REPLACE KEY VALUE CHANGED");

	verify_unmodified_tree();


	key = (int) random_upto(KEYS_COUNT - 1);

	status = red_black_tree_replace(&tree,
					(void *) (intptr_t) key,
					&replaced_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "COULDN'T REPLACE KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE(key,
				      (int) (intptr_t) replaced_key,
				      "REPLACED KEY NOT EQUAL");

	verify_unmodified_tree();
}

void
test_red_black_tree_replace_min(void)
{
	void *replaced_key;
	bool status;

	status = red_black_tree_replace_min(&tree,
					    (void *) INT_MAX,
					    &replaced_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      (int) (intptr_t) replaced_key,
				      "UNEXPECTED REPLACE MIN KEY");

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID TREE");

	status = red_black_tree_replace_min(&tree,
					    (void *) 0,
					    &replaced_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MAX,
				      (int) (intptr_t) replaced_key,
				      "REPLACED KEY NOT EQUAL");

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID TREE");

	verify_unmodified_tree();
}

void
test_red_black_tree_replace_max(void)
{
	void *replaced_key;
	bool status;

	status = red_black_tree_replace_max(&tree,
					    (void *) INT_MIN,
					    &replaced_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT - 1,
				      (int) (intptr_t) replaced_key,
				      "UNEXPECTED REPLACE MAX KEY");

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID TREE");

	status = red_black_tree_replace_max(&tree,
					    (void *) (KEYS_COUNT - 1),
					    &replaced_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MIN,
				      (int) (intptr_t) replaced_key,
				      "REPLACED KEY NOT EQUAL");

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID TREE");

	verify_unmodified_tree();
}


void
test_red_black_tree_get(void)
{
	int i;
	int key;
	int got_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		got_key = (int) (intptr_t)
			  red_black_tree_get(&tree,
					     (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      got_key,
					      "GOT UNEXPECTED KEY");
	}

	verify_unmodified_tree();
}

void
test_red_black_tree_get_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_tree_get_min(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "GOT UNEXPECTED MIN KEY");

		red_black_tree_subtract_min(&tree);
	}

	verify_empty_tree();
}

void
test_red_black_tree_get_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_tree_get_max(&tree);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "GOT UNEXPECTED MAX KEY");

		red_black_tree_subtract_max(&tree);
	}

	verify_empty_tree();
}


void
test_red_black_tree_set(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		red_black_tree_set(&tree,
				   (void *) (intptr_t) key);
	}

	verify_unmodified_tree();
}

void
test_red_black_tree_set_min(void)
{
	bool status;

	red_black_tree_set_min(&tree,
			       (void *) INT_MAX);

	status = red_black_tree_verify(&tree);


	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID TREE");

	red_black_tree_set_min(&tree,
			       (void *) 0);

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID TREE");

	verify_unmodified_tree();
}

void
test_red_black_tree_set_max(void)
{
	bool status;

	red_black_tree_set_max(&tree,
			       (void *) INT_MIN);

	status = red_black_tree_verify(&tree);


	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID TREE");

	red_black_tree_set_max(&tree,
			       (void *) (KEYS_COUNT - 1));

	status = red_black_tree_verify(&tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID TREE");

	verify_unmodified_tree();
}


void
test_red_black_tree_swap(void)
{
	int i;
	int key;
	int swapped_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		swapped_key = (int) (intptr_t)
			      red_black_tree_swap(&tree,
						  (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      swapped_key,
					      "UNEXPECTED SWAP KEY");
	}

	verify_unmodified_tree();
}

void
test_red_black_tree_swap_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t)
		      red_black_tree_swap_min(&tree,
					      (void *) INT_MAX);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MIN KEY");

		key = (int) (intptr_t)
		      red_black_tree_swap_min(&tree,
					      (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MAX,
					      key,
					      "UNEXPECTED MIN KEY");

		red_black_tree_subtract_min(&tree);
	}

	verify_empty_tree();
}

void
test_red_black_tree_swap_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t)
		      red_black_tree_swap_max(&tree,
					      (void *) INT_MIN);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MAX KEY");

		key = (int) (intptr_t)
		      red_black_tree_swap_max(&tree,
					      (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MIN,
					      key,
					      "UNEXPECTED MAX KEY");

		red_black_tree_subtract_max(&tree);
	}

	verify_empty_tree();
}
