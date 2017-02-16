#include "unity.h"          /* UNITY testing framework */
#include "red_black_hmap.h" /* RedBlackHMap, struct RedBlackNode */
#include "red_black_test.h" /* keys */


static RedBlackHMap hmap;
static RedBlackHMap hmap_copy;


static inline void
verify_empty_hmap(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_empty(&hmap),
				 "NON-EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_hmap_count(&hmap),
				       "NON-EMPTY HASH MAP");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID HASH MAP");
}


static inline void
verify_unmodified_hmap(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_congruent(&hmap_copy,
							  &hmap),
				 "HASH MAP MODIFIED FROM ORIGINAL STATE");
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

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_hmap_count(&hmap),
				       "UNEXPECTED COUNT");

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


void
test_red_black_hmap_find(void)
{
	int *restrict key;
	bool status;
	int negative_key;

	negative_key = -1;

	status = red_black_hmap_find(&hmap,
				     (void *) &negative_key,
				     sizeof(negative_key));

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED KEY");

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_find(&hmap,
					     (void *) key,
					     sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND KEY");
	}

	verify_unmodified_hmap();
}


void
test_red_black_hmap_fetch(void)
{
	int *restrict key;
	void *fetched_key = NULL;
	void *fetched_key_initial;
	bool status;
	int negative_key;

	negative_key = -1;

	fetched_key_initial = fetched_key;

	status = red_black_hmap_fetch(&hmap,
				      (void *) &negative_key,
				      sizeof(negative_key),
				      &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_PTR_MESSAGE(fetched_key_initial,
				      fetched_key,
				      "FETCHED KEY VALUE CHANGED");

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_fetch(&hmap,
					      (void *) key,
					      sizeof(*key),
					      &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(*key,
					      *((int *) fetched_key),
					      "FETCHED KEY DOES NOT MATCH");
	}

	verify_unmodified_hmap();
}


void
test_red_black_hmap_replace(void)
{
	int key;
	bool status;
	int negative_key;

	negative_key = -1;

	status = red_black_hmap_replace(&hmap,
					(void *) &negative_key,
					sizeof(negative_key));

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "REPLACED UNEXPECTED KEY");

	verify_unmodified_hmap();


	key = (int) random_upto(KEYS_COUNT - 1);

	status = red_black_hmap_replace(&hmap,
					(void *) &key,
					sizeof(key));

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "COULDN'T REPLACE KEY");

	verify_unmodified_hmap();
}


void
test_red_black_hmap_exchange(void)
{
	int key;
	void *exchanged_key = NULL;
	void *exchanged_key_initial;
	bool status;

	exchanged_key_initial = exchanged_key;

	key = -1;

	status = red_black_hmap_exchange(&hmap,
					 (void *) &key,
					 sizeof(key),
					 &exchanged_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EXCHANGED UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_PTR_MESSAGE(exchanged_key_initial,
				      exchanged_key,
				      "EXCHANGE KEY VALUE CHANGED");

	verify_unmodified_hmap();


	key = (int) random_upto(KEYS_COUNT - 1);

	status = red_black_hmap_exchange(&hmap,
					 (void *) &key,
					 sizeof(key),
					 &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "COULDN'T EXCHANGE KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE(key,
				      *((int *) exchanged_key),
				      "EXCHANGED KEY NOT EQUAL");

	verify_unmodified_hmap();
}


void
test_red_black_hmap_get(void)
{
	int *restrict key;
	int *restrict got_key;

	for (key = &keys[0]; key < keys_until; ++key) {
		got_key = (int *) red_black_hmap_get(&hmap,
						     (void *) key,
						     sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(*key,
					      *got_key,
					      "GOT UNEXPECTED KEY");
	}

	verify_unmodified_hmap();
}


void
test_red_black_hmap_set(void)
{
	int *restrict key;

	for (key = &keys[0]; key < keys_until; ++key)
		red_black_hmap_set(&hmap,
				   (void *) key,
				   sizeof(*key));

	verify_unmodified_hmap();
}


void
test_red_black_hmap_swap(void)
{
	int key;
	int *restrict swapped_key;

	for (key = 0; key < KEYS_COUNT; ++key) {
		swapped_key = (int *) red_black_hmap_swap(&hmap,
							  (void *) &key,
							  sizeof(key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      *swapped_key,
					      "UNEXPECTED SWAP KEY");

		/* replace so key isn't clobbered */
		red_black_hmap_set(&hmap,
				   swapped_key,
				   sizeof(*swapped_key));
	}

	verify_unmodified_hmap();
}
