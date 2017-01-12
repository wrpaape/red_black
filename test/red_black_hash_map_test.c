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
		EXIT_ON_SYS_FAILURE("OUT OF MEMORY");

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
		EXIT_ON_SYS_FAILURE("LOCK FAILURE");
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
test_single_thread_operations(void)
{
	ENTER("test_single_thread_operations");

	test_count(0);

	test_single_thread_insert();

	test_count(KEYS_COUNT);

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
