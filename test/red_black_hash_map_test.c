#include "red_black_test.h"	/* print macros */
#include "red_black_hash_map.h" /* red_hash_map_insert|find|verify|delete */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackHashMap hash_map;


/* macros
 * ────────────────────────────────────────────────────────────────────────── */
#define WORKERS_COUNT	4
#define REG_INTERVAL	(KEYS_COUNT / WORKERS_COUNT)
#define LONG_INTERVAL	(REG_INTERVAL + (KEYS_COUNT % WORKERS_COUNT))

struct KeyInterval {
	int from;
	int until;
};

/* static struct KeyInterval intervals[WORKERS_COUNT] = { */
/* 	[0] = */
/* }; */

static inline void
setup(void)
{
	struct sigaction action;

	ENTER("setup");

	seed_random();

	init_keys();

	if (red_black_hash_map_init(&hash_map) < 0)
		SYS_FAILURE("hash_map_init",
			    "OUT OF MEMORY");

	RETURN("setup");
}

static inline void
teardown(void)
{
	ENTER("teardown");

	red_black_hash_map_destroy(&hash_map);

	RETURN("teardown");
}

static inline void
test_count(const int expected)
{
	ENTER("test_count");

	const int count = red_black_hash_map_count(&hash_map);

	if (count < 0)
		SYS_FAILURE("hash_map_count",
			    "LOCK FAILURE");
	else if (count < expected)
		TEST_FAILURE("hash_map_count",
			     "COUNT LESS THAN EXPECTED: %u < %u",
			     count,
			     expected);
	else if (count > expected)
		TEST_FAILURE("hash_map_count",
			     "COUNT GREATER THAN EXPECTED: %u > %u",
			     count,
			     expected);

	TEST_PASS("count");

	RETURN("test_count");
}

static inline void
test_single_thread_insert(void)
{
	int *restrict key;
	int status;

	ENTER("test_single_thread_insert");

	/* shuffle_keys(); */

	key = &keys[0];

	do {
		status = red_black_hash_map_insert(&hash_map,
						   (void *) key,
						   sizeof(*key));

		if (status < 0) {
			if (status == -1)
				SYS_FAILURE("hash_map_insert",
					    "OUT OF MEMORY");
			else
				SYS_FAILURE("hash_map_insert",
					    "LOCK FAILURE");

		} else if (status == 0) {
			TEST_FAILURE("hash_map_insert",
				     "KEY NOT UNIQUE: %d",
				     *key);
#if DO_VERIFY
		} else if (!red_black_hash_map_verify(&hash_map)) {
			TEST_FAILURE("hash_map_insert",
				     "NOT VALID HASH MAP (inserted %d)",
				     *key);
#endif /* if DO_VERIFY */
		}

		++key;
	} while (key < keys_until);

	status = red_black_hash_map_insert(&hash_map,
					   (void *) &keys[0],
					   sizeof(keys[0]));

	if (status < 0) {
		if (status == -1)
			SYS_FAILURE("hash_map_insert",
				    "OUT OF MEMORY");
		else
			SYS_FAILURE("hash_map_insert",
				    "LOCK FAILURE");

	} else if (status == 1) {
		TEST_FAILURE("hash_map_insert",
			     "KEY INSERTED TWICE");
#if DO_VERIFY
	} else if (!red_black_hash_map_verify(&hash_map)) {
		TEST_FAILURE("hash_map_insert",
			     "NOT VALID HASH MAP (no insertion)");
#endif /* if DO_VERIFY */
	}

	TEST_PASS("single_thread_insert");

	RETURN("test_single_thread_insert");
}


static inline void
test_single_thread_find(void)
{
	int *restrict key;
	int unused_key;

	int status;

	ENTER("test_single_thread_find");

	shuffle_keys();

	key = &keys[0];

	do {
		status = red_black_hash_map_find(&hash_map,
						 (void *) key,
						 sizeof(*key));

		printf("got status: %d\n", status);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_find",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_find",
					     "KEY NOT FOUND: %d",
					     *key);
		}

		++key;
	} while (key < keys_until);

	unused_key = -1;


	status = red_black_hash_map_find(&hash_map,
					 (void *) &unused_key,
					 sizeof(unused_key));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("hash_map_find",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("hash_map_find",
				     "FOUND UNUSED KEY");
	}

	TEST_PASS("single_thread_find");

	RETURN("test_single_thread_find");
}


static inline void
test_single_thread_iterator(void)
{
	int last_key;
	int *restrict key;
	unsigned int count;
	int status;

	static bool key_set[KEYS_COUNT];
	bool *restrict key_set_ptr;

	RedBlackHashMapIterator iterator;

	ENTER("test_single_thread_iterator");

	/* test iterator */
	if (red_black_hash_map_iterator_init(&iterator,
					     &hash_map) < 0)
				SYS_FAILURE("hash_map_iterator_init",
					    "LOCK FAILURE");

	count = 0;

	while (1) {
		status = red_black_hash_map_iterator_next(&iterator,
							  (void **) &key);

		if (status == 0)
			break;

		if (status < 0)
			SYS_FAILURE("hash_map_iterator_next",
				    "LOCK FAILURE");

		/* got next key */
		key_set_ptr = &key_set[*key];

		if (*key_set_ptr)
			TEST_FAILURE("hash_map_iterator_next",
				     "ITERATOR TRAVERSED SAME KEY TWICE");

		*key_set_ptr = true;
		++count;
	}

	if (count < KEYS_COUNT)
		TEST_FAILURE("hash_map_iterator",
			     "ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("hash_map_iterator",
			     "ITERATOR TRAVERSED MORE THAN " KC_STR " KEYS");

	TEST_PASS("single_thread_iterator");

	RETURN("test_single_thread_iterator");
}


static inline void
test_single_thread_operations(void)
{
	ENTER("test_single_thread_operations");

	test_count(0);

	test_single_thread_insert();

	test_count(KEYS_COUNT);

	test_single_thread_find();

	test_count(KEYS_COUNT);

	test_single_thread_iterator();

	RETURN("test_single_thread_operations");
}


static inline void
test_multi_thread_operations(void)
{
	ENTER("test_multi_thread_operations");

	RETURN("test_multi_thread_operations");
}

int
main(void)
{
	setup();

	STARTING_TESTS();

	test_single_thread_operations();

	test_multi_thread_operations();

	ALL_TESTS_PASSED();

	teardown();

	return 0;
}
