#include "unity.h"          /* UNITY testing framework */
#include "red_black_tree.h" /* Tree, Etor, Itor, Node, Comparator */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <stdint.h>	    /* intptr_t */

static RedBlackTree tree;
static RedBlackTree tree_copy;
static RedBlackTreeEtor etor;
static RedBlackTreeItor itor;


static inline void
verify_unmodified_tree(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_congruent(&tree_copy,
							  &tree),
				 "TREE MODIFIED FROM ORIGINAL STATE");
}


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

		status = red_black_tree_put(&tree,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	TEST_ASSERT_FALSE_MESSAGE(red_black_tree_empty(&tree),
				  "EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_tree_count(&tree),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree),
				 "INVALID TREE");

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

	verify_unmodified_tree();
}


void
test_red_black_tree_desc_etor(void)
{
	int i;
	int j;
	void *key;
	bool status;

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

	verify_unmodified_tree();
}


static inline void
do_test_red_black_tree_itor_drop(const int delta)
{
#if 0
	int i;
	int j;
	unsigned int count;
	void *next_key;
	bool status;


	verify_unmodified_tree();

	while (red_black_tree_itor_next(&itor,
					&key)) {
		TEST_ASSERT_FALSE_MESSAGE(key & 1, /* ensure even */
					  "UNEXPECTED ODD KEY");

		red_black_tree_itor_drop(&itor); /* drop even keys */

		status = red_black_tree_itor_verify(&itor,
						    &tree);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "INVALID ITOR");

		if (!red_black_tree_itor_next(&itor,
					      &next_key))
		    break;


		TEST_ASSERT_EQUAL_INT_MESSAGE(((int) (intptr_t) key) + delta,
					      (int) (intptr_t) next_key,
					      "KEYS OUT OF ORDER");
       }

       count = red_black_tree_count(&tree);

       TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT / 2,
				     count,
				     "UNEXPECTED COUNT");

	red_black_tree_itor_reset(&itor,
				  &tree);

	while (red_black_tree_itor_next(&itor,
					&key)) {
		TEST_ASSERT_TRUE_MESSAGE(key & 1, /* ensure odd */
					 "EVEN KEY REMAINS");

		red_black_tree_itor_drop(&itor); /* drop rest of keys */

		status = red_black_tree_itor_verify(&itor,
						    &tree);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "INVALID ITOR");
	}

	verify_empty_tree();
#endif
}


void
test_red_black_tree_asc_itor(void)
{
	int i;
	int j;
	void *key;
	bool status;

	red_black_tree_asc_itor_init(&itor,
				     &tree);

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < KEYS_COUNT; ++j) {
			status = red_black_tree_itor_verify(&itor,
							    &tree);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "INVALID ASC ITERATOR");

			status = red_black_tree_itor_current(&itor,
							     &key);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "ASC ITERATOR SKIPPED KEYS");

			TEST_ASSERT_EQUAL_INT_MESSAGE(j,
						      (int) (intptr_t) key,
						      "UNEXPECTED CURRENT KEY");

			red_black_tree_itor_skip(&itor);
		}

		status = red_black_tree_itor_current(&itor,
						     &key);

		TEST_ASSERT_FALSE_MESSAGE(status,
					  "KEYS REMAIN AFTER ASC ITERATION");

		red_black_tree_itor_reset(&itor,
					  &tree);
	}

	/* do_test_red_black_tree_itor_drop(1); */
}


void
test_red_black_tree_desc_itor(void)
{
	int i;
	int j;
	void *key;
	bool status;
	RedBlackTreeItor itor;

	red_black_tree_desc_itor_init(&itor,
				      &tree);

	for (i = 0; i < 2; ++i) {
		for (j = KEYS_COUNT - 1; j >= 0; --j) {
			status = red_black_tree_itor_verify(&itor,
							    &tree);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "INVALID DESC ITERATOR");

			status = red_black_tree_itor_current(&itor,
							     &key);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "DESC ITERATOR SKIPPED KEYS");

			TEST_ASSERT_EQUAL_INT_MESSAGE(j,
						      (int) (intptr_t) key,
						      "UNEXPECTED CURRENT KEY");

			red_black_tree_itor_skip(&itor);
		}

		status = red_black_tree_itor_current(&itor,
						     &key);

		TEST_ASSERT_FALSE_MESSAGE(status,
					  "KEYS REMAIN AFTER DESC ITERATION");

		red_black_tree_itor_reset(&itor,
					  &tree);
	}

#if ((KEYS_COUNT > 0) && ((KEYS_COUNT - 1) & 1)) /* ensure 1st entry is even */
	red_black_tree_itor_skip(&itor);
#endif

	/* do_test_red_black_tree_itor_drop(-1); */
}
