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

	status = red_black_hmap_similar(&asc_hmap,
					&desc_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "ASC/DESC HASH MAPS NOT SIMILAR");

	status = red_black_hmap_congruent(&asc_hmap,
					  &desc_hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "ASC/DESC HASH MAPS UNEXPECTEDLY CONGRUENT");
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
