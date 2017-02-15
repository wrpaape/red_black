#include "unity.h"             /* UNITY testing framework */
#include "red_black_hmap.h"    /* RedBlackHMap, struct RedBlackHNode */
#include "red_black_test.h"    /* keys */



static RedBlackHMap hmap;

static inline void
verify_full_hmap(void)
{
	TEST_ASSERT_FALSE_MESSAGE(red_black_hmap_empty(&hmap),
				  "EMPTY HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_hmap_count(&hmap),
				       "UNEXPECTED COUNT");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID HASH MAP");
}

void
setUp(void)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_init(&hmap),
				 "OUT OF MEMORY");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID HASH MAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_hmap_count(&hmap),
				       "NON-EMPTY HASH MAP");

	shuffle_keys();
}

void
tearDown(void)
{
	verify_full_hmap();

	red_black_hmap_destroy(&hmap);
}


void
test_red_black_hmap_insert(void)
{
	int status;
	int *restrict key;

	for (key = &keys[0]; key < keys_until; ++key) {

		status = red_black_hmap_insert(&hmap,
					       (void *) key,
					       sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");
	}

	verify_full_hmap();

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_insert(&hmap,
					       (void *) key,
					       sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");
	}
}


void
test_red_black_hmap_put(void)
{
	int status;
	int *restrict key;

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_put(&hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");
	}

	verify_full_hmap();

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_put(&hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");
	}
}


void
test_red_black_hmap_update(void)
{
	int status;
	int *restrict key;
	void *old_key = NULL;
	void *old_key_initial;

	for (key = &keys[0]; key < keys_until; ++key) {
		old_key_initial = old_key;

		status = red_black_hmap_update(&hmap,
					       (void *) key,
					       sizeof(*key),
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "INSERTED USED KEY (0)"
					      " OR OUT OF MEMORY (-1)");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(old_key_initial,
					      old_key,
					      "VALUE OF OLD KEY CHANGED");
	}

	verify_full_hmap();

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_update(&hmap,
					       (void *) key,
					       sizeof(*key),
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-INSERTED USED KEY (1)"
					      " OR OUT OF MEMORY (-1)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(*key,
					      *((int *) old_key),
					      "OLD KEY DOES NOT MATCH");
	}
}


void
test_red_black_hmap_add(void)
{
	int *restrict key;
	bool status;

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_add(&hmap,
					    (void *) key,
					    sizeof(*key));

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}
}
