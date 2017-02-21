#include "unity.h"          /* UNITY testing framework */
#include "red_black_hmap.h" /* RedBlackHMap, struct RedBlackNode */
#include "red_black_test.h" /* keys */


static RedBlackHMap asc_hmap;
static RedBlackHMap desc_hmap;


static inline void
init_hmap(RedBlackHMap *const restrict hmap)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_init(hmap),
				 "OUT OF MEMORY");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(hmap),
				 "INVALID HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_hmap_count(hmap),
				       "NON-EMPTY HASH MAP");
}


static inline void
verify_full_hmap(const RedBlackHMap *const restrict hmap)
{
	TEST_ASSERT_FALSE_MESSAGE(red_black_hmap_empty(hmap),
				  "EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_hmap_count(hmap),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(hmap),
				 "INVALID HASH MAP");
}


void
setUp(void)
{
	int *restrict key;
	bool status;

	init_hmap(&asc_hmap);

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_add(&asc_hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	verify_full_hmap(&asc_hmap);

	init_hmap(&desc_hmap);

	while (--key >= &keys[0]) {
		status = red_black_hmap_add(&desc_hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	verify_full_hmap(&desc_hmap);
}

void
tearDown(void)
{
	red_black_hmap_destroy(&asc_hmap);
	red_black_hmap_destroy(&desc_hmap);
}


void
test_red_black_hmap_compare(void)
{
	bool status;
	int *restrict key;
	RedBlackHMap *restrict hmap_ptr;
	RedBlackHMap positive_hmap;

	/* compare ASC hmap to DESC hmap */
	status = red_black_hmap_similar(&asc_hmap,
					&desc_hmap)
	      && red_black_hmap_similar(&desc_hmap,
					&asc_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ASC/DESC HMAPS NOT SIMILAR");

	status = red_black_hmap_congruent(&asc_hmap,
					  &desc_hmap)
	      || red_black_hmap_congruent(&desc_hmap,
					  &asc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ASC/DESC HMAPS UNEXPECTEDLY CONGRUENT");

	init_hmap(&positive_hmap);

	/* delete ODD  keys from asc_hmap
	 * delete EVEN keys from desc_hmap
	 * add    ALL  keys to   positive_hmap */

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_add(&positive_hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY")
		hmap_ptr = (*key & 1)
			 ? &asc_hmap
			 : &desc_hmap;

		red_black_hmap_drop(hmap_ptr,
				    (void *) key,
				    sizeof(*key));

	}

	/* asc_hmap	 contains only EVEN keys
	 * desc_hmap	 contains only ODD  keys
	 * positive_hmap contains ALL       keys */

	/* compare EVEN hmap to POSITIVE hmap */
	status = red_black_hmap_intersect(&positive_hmap,
					  &asc_hmap)
	      && red_black_hmap_intersect(&asc_hmap,
					  &positive_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "EVEN HMAP DOES NOT INTERSECT WITH "
				 "POSITIVE HMAP");

	status = red_black_hmap_subset(&positive_hmap,
				       &asc_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "EVEN HMAP IS NOT A SUBSET OF POSITIVE HMAP");

	status = red_black_hmap_subset(&asc_hmap,
				       &positive_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "POSITIVE HMAP IS A SUBSET OF EVEN HMAP");

	const int negative_two = -2;

	status = red_black_hmap_add(&asc_hmap,
				    (void *) &negative_two,
				    sizeof(negative_two));

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_hmap_subset(&positive_hmap,
				       &asc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN HMAP WITH NEGATIVE KEY IS A SUBSET OF "
				  "POSITIVE HMAP");

	/* compare ODD hmap to POSITIVE hmap */
	status = red_black_hmap_intersect(&positive_hmap,
					  &desc_hmap)
	      && red_black_hmap_intersect(&desc_hmap,
					  &positive_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ODD HMAP DOES NOT INTERSECT WITH "
				 "POSITIVE HMAP");

	status = red_black_hmap_subset(&positive_hmap,
				       &desc_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ODD HMAP IS NOT A SUBSET OF POSITIVE HMAP");

	status = red_black_hmap_subset(&desc_hmap,
				       &positive_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "POSITIVE HMAP IS A SUBSET OF ODD HMAP");

	const int negative_one = -1;

	status = red_black_hmap_add(&desc_hmap,
				    (void *) &negative_one,
				    sizeof(negative_one));

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_hmap_subset(&positive_hmap,
				       &desc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ODD HMAP WITH NEGATIVE KEY IS A SUBSET OF "
				  "POSITIVE HMAP");

	red_black_hmap_destroy(&positive_hmap); /* done with positive hmap */

	/* compare EVEN hmap to ODD hmap */
	status = red_black_hmap_intersect(&asc_hmap,
					  &desc_hmap)
	      || red_black_hmap_intersect(&desc_hmap,
					  &asc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN HMAP INTERSECTS WITH ODD HMAP");

	status = red_black_hmap_subset(&asc_hmap,
				       &desc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EVEN HMAP IS A SUBSET OF ODD HMAP");

	status = red_black_hmap_subset(&asc_hmap,
				       &desc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ODD HMAP IS A SUBSET OF EVEN HMAP");
}


static inline void
do_test_red_black_hmap_clone(const RedBlackHMap *const restrict other_hmap)
{
	bool status;
	RedBlackHMap hmap;

	status = red_black_hmap_clone(&hmap,
				      other_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_hmap_similar(&hmap,
					other_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OTHER HASH MAP AND CLONE NOT SIMILAR");

	status = red_black_hmap_congruent(&hmap,
					  other_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OTHER HASH MAP AND CLONE NOT CONGRUENT");

	red_black_hmap_destroy(&hmap);
}

void
test_red_black_hmap_clone(void)
{
	do_test_red_black_hmap_clone(&asc_hmap);
	do_test_red_black_hmap_clone(&desc_hmap);
}
