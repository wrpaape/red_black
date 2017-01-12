#include "red_black_test.h"	/* print macros */
#include "red_black_hash_map.h" /* red_hash_map_insert|find|verify|delete */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackHashMap hash_map;


/* macros
 * ────────────────────────────────────────────────────────────────────────── */
#define THREADS_COUNT		4

/* threads API */
#ifdef WIN32
typedef HANDLE Thread;
typedef DWORD ThreadReturn;

#define SPAWN_THREAD(THREAD, FUNC_PTR, ARG_PTR)				\
((THREAD) = CreateThread(NULL,						\
			 0,						\
			 FUNC_PTR,					\
			 ARG_PTR,					\
			 0,						\
			 NULL) != NULL)

#define WAIT_THREAD(THREAD)						\
(WaitForSingleObject(THREAD,						\
		     INFINITE) == WAIT_OBJECT_0)


#else
typedef pthread_t Thread;
typedef void *ThreadReturn;

#define SPAWN_THREAD(THREAD, FUNC_PTR, ARG_PTR)				\
(pthread_create(&(THREAD),						\
		NULL,							\
		FUNC_PTR,						\
		ARG_PTR) == 0)

#define WAIT_THREAD(THREAD)						\
(pthread_join(THREAD,							\
	      NULL) == 0)

#endif


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
struct KeyInterval {
	int *restrict from;
	const int *restrict until;
};

static const struct KeyInterval total_interval = {
	.from  = &keys[0],
	.until = &keys[KEYS_COUNT]
};

#define MID_INTERVAL_LENGTH	(KEYS_COUNT / THREADS_COUNT)
#define LAST_INTERVAL_LENGTH	(MID_INTERVAL + (KEYS_COUNT % THREADS_COUNT))

#define DEF_INTERVAL(I, FROM, UNTIL)					\
[I] = { .from = &keys[FROM], .until = &keys[UNTIL] }
#define DEF_MID_INTERVAL(I)						\
DEF_INTERVAL(I, (I) * MID_INTERVAL_LENGTH, ((I) + 1) * MID_INTERVAL_LENGTH)
#define DEF_LAST_INTERVAL(I)						\
DEF_INTERVAL(I, (I) * MID_INTERVAL_LENGTH, KEYS_COUNT)

static const struct KeyInterval intervals[THREADS_COUNT] = {
	DEF_MID_INTERVAL(0),
	DEF_MID_INTERVAL(1),
	DEF_MID_INTERVAL(2),
	DEF_LAST_INTERVAL(THREADS_COUNT - 1)
};


static Thread thread[THREADS_COUNT];



static inline void
setup(void)
{
	struct sigaction action;

	ENTER("setup");

	seed_random();

	init_keys();

	shuffle_keys();

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

static inline ThreadReturn
test_insert(void *arg)
{
	int *restrict key;
	struct KeyInterval *restrict interval;
	int status;

	ENTER("test_insert");

	interval = (struct KeyInterval *) arg;

	for (key = interval->from; key < interval->until; ++key) {
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
		} else {
			status = red_black_hash_map_verify(&hash_map);

			if (status != 1) {
				if (status < 0)
					SYS_FAILURE("hash_map_verify",
						    "LOCK FAILURE");
				else
					TEST_FAILURE("hash_map_insert",
						     "NOT VALID HASH MAP "
						     "(inserted %d)",
						     *key);
			}
#endif /* if DO_VERIFY */
		}
	}


	status = red_black_hash_map_insert(&hash_map,
					   (void *) interval->from,
					   sizeof(*(interval->from)));

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
	} else {
		status = red_black_hash_map_verify(&hash_map);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_insert",
					     "NOT VALID HASH MAP "
					     "(no insertion)");
		}
#endif /* if DO_VERIFY */
	}

	TEST_PASS("insert");

	RETURN("test_insert");

	return (ThreadReturn) 0;
}


static inline ThreadReturn
test_find(void *arg)
{
	int *restrict key;
	int unused_key;
	int status;

	/* DON'T SHUFFLE, HASH KEYS WILL CHANGE! */

	ENTER("test_find");

	key = &keys[0];

	for (key = total_interval.from; key < total_interval.until; ++key) {
		status = red_black_hash_map_find(&hash_map,
						 (void *) key,
						 sizeof(*key));

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_find",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_find",
					     "KEY NOT FOUND: %d",
					     *key);
		}
	}


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

	TEST_PASS("find");

	RETURN("test_find");

	return (ThreadReturn) 0;
}


static inline ThreadReturn
test_iterator(void *arg)
{
	int last_key;
	int *restrict key;
	unsigned int count;
	size_t length;
	int status;
	bool key_set[KEYS_COUNT] = { 0 };
	bool *restrict key_set_ptr;

	RedBlackHashMapIterator iterator;

	ENTER("test_iterator");

	/* test iterator */
	if (red_black_hash_map_iterator_init(&iterator,
					     &hash_map) < 0)
				SYS_FAILURE("hash_map_iterator_init",
					    "LOCK FAILURE");

	count = 0;

	while (1) {
		status = red_black_hash_map_iterator_next(&iterator,
							  (void **) &key,
							  &length);

		if (status == 0)
			break;

		if (status < 0)
			SYS_FAILURE("hash_map_iterator_next",
				    "LOCK FAILURE");

		if (length != sizeof(*key))
			TEST_FAILURE("hash_map_iterator_next",
				     "GOT UNEXPECTED LENGTH %zu INSTEAD OF %zu",
				     length,
				     sizeof(*key));

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

	TEST_PASS("iterator");

	RETURN("test_iterator");

	return (ThreadReturn) 0;
}


static inline ThreadReturn
test_delete(void *arg)
{
	int *restrict key;
	struct KeyInterval *restrict interval;
	int unused_key;
	int status;

	ENTER("test_delete");

	unused_key = -1;

	status = red_black_hash_map_delete(&hash_map,
					   (void *) &unused_key,
					   sizeof(unused_key));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("hash_map_delete",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("hash_map_delete",
				     "DELETED UNUSED KEY");
#if DO_VERIFY
	} else {
		status = red_black_hash_map_verify(&hash_map);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_delete",
					     "NOT VALID HASH MAP "
					     "(no deletion)");
		}
#endif /* if DO_VERIFY */
	}

	interval = (struct KeyInterval *) arg;

	key = &keys[0];

	for (key = interval->from; key < interval->until; ++key) {
		status = red_black_hash_map_delete(&hash_map,
						   (void *) key,
						   sizeof(*key));

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_delete",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_delete",
					     "KEY NOT FOUND: %d",
					     *key);
#if DO_VERIFY
		} else {
			status = red_black_hash_map_verify(&hash_map);

			if (status != 1) {
				if (status < 0)
					SYS_FAILURE("hash_map_verify",
						    "LOCK FAILURE");
				else
					TEST_FAILURE("hash_map_delete",
						     "NOT VALID HASH MAP "
						     "(deleted %d)",
						     *key);
			}
#endif /* if DO_VERIFY */
		}
	}


	status = red_black_hash_map_delete(&hash_map,
					   (void *) interval->from,
					   sizeof(*(interval->from)));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("hash_map_delete",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("hash_map_delete",
				     "DELETED KEY TWICE");
#if DO_VERIFY
	} else {
		status = red_black_hash_map_verify(&hash_map);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("hash_map_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("hash_map_delete",
					     "NOT VALID HASH MAP "
					     "(no deletion)");
		}
#endif /* if DO_VERIFY */
	}

	TEST_PASS("delete");

	RETURN("test_delete");

	return (ThreadReturn) 0;
}


static inline void
test_single_thread_operations(void)
{
	ENTER("test_single_thread_operations");

	setup();

	test_count(0);

	(void) test_insert((void *) &total_interval);

	test_count(KEYS_COUNT);

	(void) test_find(NULL);

	test_count(KEYS_COUNT);

	(void) test_iterator(NULL);

	test_count(KEYS_COUNT);

	(void) test_delete((void *) &total_interval);

	test_count(0);

	teardown();

	RETURN("test_single_thread_operations");
}


static inline void
test_multi_thread_operations(void)
{
	ENTER("test_multi_thread_operations");

	setup();

	test_count(0);

	/* test_multi_thread_insert(); */

	/* test_count(KEYS_COUNT); */

	/* test_multi_thread_find(); */

	/* test_count(KEYS_COUNT); */

	/* test_multi_thread_iterator(); */

	/* test_count(KEYS_COUNT); */

	/* test_multi_thread_delete(); */

	/* test_count(0); */

	teardown();

	RETURN("test_multi_thread_operations");
}

int
main(void)
{
	STARTING_TESTS();

	test_single_thread_operations();

	test_multi_thread_operations();

	ALL_TESTS_PASSED();

	return 0;
}
