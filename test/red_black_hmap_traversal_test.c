#include "unity.h"          /* UNITY testing framework */
#include "red_black_hmap.h" /* HMap, Itor, Itor, Node, Comparator */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <stdint.h>	    /* intptr_t */

static RedBlackHMap hmap;
static RedBlackHMap hmap_copy;
static RedBlackHMapItor itor;


static inline void
verify_unmodified_hmap(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_congruent(&hmap_copy,
							  &hmap),
				 "TREE MODIFIED FROM ORIGINAL STATE");
}


static inline void
verify_empty_hmap(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_empty(&hmap),
				 "NON-EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_hmap_count(&hmap),
				       "NON-EMPTY TREE");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID TREE");
}


void
setUp(void)
{
	int *restrict key;
	bool status;

	status = red_black_hmap_init(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	verify_empty_hmap();

	shuffle_keys();

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_add(&hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	TEST_ASSERT_FALSE_MESSAGE(red_black_hmap_empty(&hmap),
				  "EMPTY TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_hmap_count(&hmap),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID TREE");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_clone(&hmap_copy,
						      &hmap),
				 "OUT OF MEMORY");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_congruent(&hmap,
							  &hmap_copy),
				 "BAD CLONE");
}


void
tearDown(void)
{
	red_black_hmap_destroy(&hmap_copy);

	red_black_hmap_destroy(&hmap);
}


static bool key_set[KEYS_COUNT];

void
test_red_black_hmap_itor(void)
{
	int i;
	int j;
	void *key;
	void *key_inital;
	bool *restrict key_set_ptr;
	bool status;
	size_t length;
	size_t length_initial;

	red_black_hmap_itor_init(&itor,
				 &hmap);

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < KEYS_COUNT; ++j) {
			status = red_black_hmap_itor_next(&itor,
							  &key,
							  &length);

			TEST_ASSERT_TRUE_MESSAGE(status,
						 "ITERATOR SKIPPED KEYS");

			TEST_ASSERT_EQUAL_UINT_MESSAGE((unsigned int) sizeof(int),
						       (unsigned int) length,
						       "UNEXPECTED KEY LENGTH");

			key_set_ptr = &key_set[*((int *) key)];

			TEST_ASSERT_FALSE_MESSAGE(*key_set_ptr,
						  "KEY TRAVERSED TWICE");

			*key_set_ptr = true;
		}

		key_inital     = key;
		length_initial = length;

		status = red_black_hmap_itor_next(&itor,
						  &key,
						  &length);

		TEST_ASSERT_FALSE_MESSAGE(status,
					  "KEYS REMAIN AFTER ASC ITERATION");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(key_inital,
					      key,
					      "KEY VALUE CHANGED");

		TEST_ASSERT_EQUAL_UINT_MESSAGE((unsigned int) length_initial,
					       (unsigned int) length,
					      "LENGTH VALUE CHANGED");

		red_black_hmap_itor_reset(&itor,
					  &hmap);

		for (j = 0; j < KEYS_COUNT; ++j)
			key_set[j] = false;
	}

	verify_unmodified_hmap();
}
