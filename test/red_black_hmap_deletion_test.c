#include "unity.h"             /* UNITY testing framework */
#include "red_black_hmap.h"    /* RedBlackHMap */
#include "red_black_test.h"    /* keys */


static RedBlackHMap hmap;

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

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(&hmap),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT,
				       red_black_hmap_count(&hmap),
				       "UNEXPECTED COUNT");
}

void
tearDown(void)
{
	verify_empty_hmap();

	red_black_hmap_destroy(&hmap);
}


void
test_red_black_hmap_delete(void)
{
	int status;
	int *restrict key;

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_delete(&hmap,
					       (void *) key,
					       sizeof(*key));

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "DELETED KEY TWICE (0)");
	}
}


void
test_red_black_hmap_remove(void)
{
	int status;
	int *restrict key;
	int key_value;
	void *old_key;
	void *old_key_initial;

	for (key = &keys[0]; key < keys_until; ++key) {
		status = red_black_hmap_remove(&hmap,
					       (void *) key,
					       sizeof(*key),
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(1,
					      status,
					      "REMOVED KEY TWICE (0)");

		TEST_ASSERT_EQUAL_INT_MESSAGE(*key,
					      *((int *) old_key),
					      "OLD KEY DOES NOT MATCH");
	}

	verify_empty_hmap();

	for (key_value = 0; key_value < KEYS_COUNT; ++key_value) {
		old_key_initial = old_key;

		status = red_black_hmap_remove(&hmap,
					       (void *) &key_value,
					       sizeof(key_value),
					       &old_key);

		TEST_ASSERT_EQUAL_INT_MESSAGE(0,
					      status,
					      "RE-REMOVED USED KEY (1)");

		TEST_ASSERT_EQUAL_PTR_MESSAGE(old_key_initial,
					      old_key,
					      "VALUE OF OLD KEY CHANGED");
	}
}


void
test_red_black_hmap_drop(void)
{
	int *restrict key;

	for (key = &keys[0]; key < keys_until; ++key)
		red_black_hmap_drop(&hmap,
				    (void *) key,
				    sizeof(*key));
}
