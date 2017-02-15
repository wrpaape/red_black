#include "unity.h"          /* UNITY testing framework */
#include "red_black_hmap.h" /* RedBlackHMap, struct RedBlackNode */
#include "int_key.h"        /* int_key_comparator */
#include "red_black_test.h" /* keys */
#include <limits.h>	    /* INT_MIN|MAX */

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

	red_black_hmap_init(&hmap,
			    &int_key_comparator);

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

	shuffle_keys();
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
	int i;
	int key;
	void *fetched_key = NULL;
	void *fetched_key_initial;
	bool status;

	fetched_key_initial = fetched_key;

	status = red_black_hmap_fetch(&hmap,
				      (void *) -1,
				      &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCHED KEY VALUE CHANGED");

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_hmap_fetch(&hmap,
					      (void *) (intptr_t) key,
					      &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      (int) (intptr_t) fetched_key,
					      "FETCHED KEY DOES NOT MATCH");
	}

	verify_unmodified_hmap();
}


void
test_red_black_hmap_fetch_min(void)
{
	int i;
	void *fetched_key;
	void *fetched_key_initial;
	bool status;

	for (i = 0; i < KEYS_COUNT; ++i) {
		status = red_black_hmap_fetch_min(&hmap,
						  &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND MIN KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) fetched_key,
					      "FETCHED MIN KEY DOES NOT MATCH");

		red_black_hmap_drop_min(&hmap);
	}

	verify_empty_hmap();

	fetched_key_initial = fetched_key;

	status = red_black_hmap_fetch_min(&hmap,
					  &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED MIN KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCH KEY VALUE CHANGED");
}


void
test_red_black_hmap_fetch_max(void)
{
	int i;
	void *fetched_key;
	void *fetched_key_initial;
	bool status;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		status = red_black_hmap_fetch_max(&hmap,
						  &fetched_key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "DIDN'T FIND MAX KEY");

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      (int) (intptr_t) fetched_key,
					      "FETCHED MAX KEY DOES NOT MATCH");

		red_black_hmap_drop_max(&hmap);
	}

	verify_empty_hmap();

	fetched_key_initial = fetched_key;

	status = red_black_hmap_fetch_max(&hmap,
					  &fetched_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "FOUND UNEXPECTED MAX KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) fetched_key_initial,
				      (int) (intptr_t) fetched_key,
				      "FETCH KEY VALUE CHANGED");
}

void
test_red_black_hmap_replace(void)
{
	int key;
	bool status;

	status = red_black_hmap_replace(&hmap,
					(void *) -1);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "REPLACED UNEXPECTED KEY");

	verify_unmodified_hmap();


	key = (int) random_upto(KEYS_COUNT - 1);

	status = red_black_hmap_replace(&hmap,
					(void *) (intptr_t) key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "COULDN'T REPLACE KEY");

	verify_unmodified_hmap();
}

void
test_red_black_hmap_replace_min(void)
{
	bool status;

	status = red_black_hmap_replace_min(&hmap,
					    (void *) INT_MAX);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY HASH MAP");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	status = red_black_hmap_replace_min(&hmap,
					    (void *) 0);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY HASH MAP");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

	verify_unmodified_hmap();
}

void
test_red_black_hmap_replace_max(void)
{
	bool status;

	status = red_black_hmap_replace_max(&hmap,
					    (void *) INT_MIN);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY HASH MAP");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	status = red_black_hmap_replace_max(&hmap,
					    (void *) (KEYS_COUNT - 1));

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO REPLACE IN NON-EMPTY HASH MAP");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

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

	status = red_black_hmap_exchange(&hmap,
					 (void *) -1,
					 &exchanged_key);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "EXCHANGED UNEXPECTED KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE((int) (intptr_t) exchanged_key_initial,
				      (int) (intptr_t) exchanged_key,
				      "EXCHANGE KEY VALUE CHANGED");

	verify_unmodified_hmap();


	key = (int) random_upto(KEYS_COUNT - 1);

	status = red_black_hmap_exchange(&hmap,
					 (void *) (intptr_t) key,
					 &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "COULDN'T EXCHANGE KEY");

	TEST_ASSERT_EQUAL_INT_MESSAGE(key,
				      (int) (intptr_t) exchanged_key,
				      "EXCHANGED KEY NOT EQUAL");

	verify_unmodified_hmap();
}

void
test_red_black_hmap_exchange_min(void)
{
	void *exchanged_key;
	bool status;

	status = red_black_hmap_exchange_min(&hmap,
					     (void *) INT_MAX,
					     &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO EXCHANGE IN NON-EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      (int) (intptr_t) exchanged_key,
				      "UNEXPECTED EXCHANGE MIN KEY");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	status = red_black_hmap_exchange_min(&hmap,
					     (void *) 0,
					     &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO EXCHANGE IN NON-EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MAX,
				      (int) (intptr_t) exchanged_key,
				      "EXCHANGED KEY NOT EQUAL");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

	verify_unmodified_hmap();
}

void
test_red_black_hmap_exchange_max(void)
{
	void *exchanged_key;
	bool status;

	status = red_black_hmap_exchange_max(&hmap,
					     (void *) INT_MIN,
					     &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO EXCHANGE IN NON-EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT - 1,
				      (int) (intptr_t) exchanged_key,
				      "UNEXPECTED EXCHANGE MAX KEY");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	status = red_black_hmap_exchange_max(&hmap,
					     (void *) (KEYS_COUNT - 1),
					     &exchanged_key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "FAILED TO EXCHANGE IN NON-EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MIN,
				      (int) (intptr_t) exchanged_key,
				      "EXCHANGED KEY NOT EQUAL");

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

	verify_unmodified_hmap();
}


void
test_red_black_hmap_get(void)
{
	int i;
	int key;
	int got_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		got_key = (int) (intptr_t)
			  red_black_hmap_get(&hmap,
					     (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      got_key,
					      "GOT UNEXPECTED KEY");
	}

	verify_unmodified_hmap();
}

void
test_red_black_hmap_get_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t) red_black_hmap_get_min(&hmap);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "GOT UNEXPECTED MIN KEY");

		red_black_hmap_drop_min(&hmap);
	}

	verify_empty_hmap();
}

void
test_red_black_hmap_get_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t) red_black_hmap_get_max(&hmap);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "GOT UNEXPECTED MAX KEY");

		red_black_hmap_drop_max(&hmap);
	}

	verify_empty_hmap();
}


void
test_red_black_hmap_set(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		red_black_hmap_set(&hmap,
				   (void *) (intptr_t) key);
	}

	verify_unmodified_hmap();
}

void
test_red_black_hmap_set_min(void)
{
	bool status;

	red_black_hmap_set_min(&hmap,
			       (void *) INT_MAX);

	status = red_black_hmap_verify(&hmap);


	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	red_black_hmap_set_min(&hmap,
			       (void *) 0);

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

	verify_unmodified_hmap();
}

void
test_red_black_hmap_set_max(void)
{
	bool status;

	red_black_hmap_set_max(&hmap,
			       (void *) INT_MIN);

	status = red_black_hmap_verify(&hmap);


	TEST_ASSERT_FALSE_MESSAGE(status,
				  "UNEXPECTEDLY VALID HASH MAP");

	red_black_hmap_set_max(&hmap,
			       (void *) (KEYS_COUNT - 1));

	status = red_black_hmap_verify(&hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "UNEXPECTEDLY INVALID HASH MAP");

	verify_unmodified_hmap();
}


void
test_red_black_hmap_swap(void)
{
	int i;
	int key;
	int swapped_key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		swapped_key = (int) (intptr_t)
			      red_black_hmap_swap(&hmap,
						  (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(key,
					      swapped_key,
					      "UNEXPECTED SWAP KEY");
	}

	verify_unmodified_hmap();
}

void
test_red_black_hmap_swap_min(void)
{
	int i;
	int key;

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = (int) (intptr_t)
		      red_black_hmap_swap_min(&hmap,
					      (void *) INT_MAX);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MIN KEY");

		key = (int) (intptr_t)
		      red_black_hmap_swap_min(&hmap,
					      (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MAX,
					      key,
					      "UNEXPECTED MIN KEY");

		red_black_hmap_drop_min(&hmap);
	}

	verify_empty_hmap();
}

void
test_red_black_hmap_swap_max(void)
{
	int i;
	int key;

	for (i = KEYS_COUNT - 1; i >= 0; --i) {
		key = (int) (intptr_t)
		      red_black_hmap_swap_max(&hmap,
					      (void *) INT_MIN);

		TEST_ASSERT_EQUAL_INT_MESSAGE(i,
					      key,
					      "UNEXPECTED MAX KEY");

		key = (int) (intptr_t)
		      red_black_hmap_swap_max(&hmap,
					      (void *) (intptr_t) key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(INT_MIN,
					      key,
					      "UNEXPECTED MAX KEY");

		red_black_hmap_drop_max(&hmap);
	}

	verify_empty_hmap();
}
