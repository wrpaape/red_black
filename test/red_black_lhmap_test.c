#include "red_black_test.h"  /* print/exit macros */
#include "red_black_lhmap.h" /* red_lhmap_insert|find|verify|delete */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackLHMap lhmap;


/* threads API
 * ────────────────────────────────────────────────────────────────────────── */
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

typedef ThreadReturn
(*ThreadRoutine)(void *arg);


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
struct KeyInterval {
	int *restrict from;
	const int *restrict until;
};

#define THREADS_COUNT		4

#define Q0 0
#define Q1 (KEYS_COUNT / THREADS_COUNT)
#define Q2 (2 * Q1)
#define Q3 (3 * Q1)
#define Q4 KEYS_COUNT

static const struct KeyInterval intervals[THREADS_COUNT] = {
	[0] = { .from = &keys[Q0], .until = &keys[Q1] },
	[1] = { .from = &keys[Q1], .until = &keys[Q2] },
	[2] = { .from = &keys[Q2], .until = &keys[Q3] },
	[3] = { .from = &keys[Q3], .until = &keys[Q4] }
};

static const struct KeyInterval total_interval = {
	.from  = &keys[Q0],
	.until = &keys[Q4]
};


static inline void
setup(void)
{
	ENTER("setup");

	init_keys();

	seed_random();

	shuffle_keys();

	if (red_black_lhmap_init(&lhmap) < 0)
		SYS_FAILURE("lhmap_init",
			    "OUT OF MEMORY");

	RETURN("setup");
}

static inline void
teardown(void)
{
	ENTER("teardown");

	red_black_lhmap_destroy(&lhmap);

	RETURN("teardown");
}

static inline void
test_count(const int expected)
{
	ENTER("test_count");

	const int count = red_black_lhmap_count(&lhmap);

	if (count < 0)
		SYS_FAILURE("lhmap_count",
			    "LOCK FAILURE");
	else if (count < expected)
		TEST_FAILURE("lhmap_count",
			     "COUNT LESS THAN EXPECTED: %u < %u",
			     count,
			     expected);
	else if (count > expected)
		TEST_FAILURE("lhmap_count",
			     "COUNT GREATER THAN EXPECTED: %u > %u",
			     count,
			     expected);

	TEST_PASS("count");

	RETURN("test_count");
}

ThreadReturn
test_insert(void *arg)
{
	int *restrict key;
	struct KeyInterval *restrict interval;
	int status;

	ENTER("test_insert");

	interval = (struct KeyInterval *) arg;

	for (key = interval->from; key < interval->until; ++key) {
		status = red_black_lhmap_insert(&lhmap,
						(void *) key,
						sizeof(*key));

		/* printf("%d\n", *key); fflush(stdout); */

		if (status < 0) {
			if (status == -1)
				SYS_FAILURE("lhmap_insert",
					    "OUT OF MEMORY");
			else
				SYS_FAILURE("lhmap_insert",
					    "LOCK FAILURE");

		} else if (status == 0) {
			TEST_FAILURE("lhmap_insert",
				     "KEY NOT UNIQUE: %d",
				     *key);
#if DO_VERIFY
		} else {
			status = red_black_lhmap_verify(&lhmap);

			if (status != 1) {
				if (status < 0)
					SYS_FAILURE("lhmap_verify",
						    "LOCK FAILURE");
				else
					TEST_FAILURE("lhmap_insert",
						     "NOT VALID HASH MAP "
						     "(inserted %d)",
						     *key);
			}
#endif /* if DO_VERIFY */
		}
	}


	status = red_black_lhmap_insert(&lhmap,
					(void *) interval->from,
					sizeof(*(interval->from)));

	if (status < 0) {
		if (status == -1)
			SYS_FAILURE("lhmap_insert",
				    "OUT OF MEMORY");
		else
			SYS_FAILURE("lhmap_insert",
				    "LOCK FAILURE");

	} else if (status == 1) {
		TEST_FAILURE("lhmap_insert",
			     "KEY INSERTED TWICE");
#if DO_VERIFY
	} else {
		status = red_black_lhmap_verify(&lhmap);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("lhmap_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("lhmap_insert",
					     "NOT VALID HASH MAP "
					     "(no insertion)");
		}
#endif /* if DO_VERIFY */
	}

	TEST_PASS("insert");

	RETURN("test_insert");

	return (ThreadReturn) 0;
}


ThreadReturn
test_find(void *arg)
{
	int *restrict key;
	int unused_key;
	int status;

	/* DON'T SHUFFLE, HASH KEYS WILL CHANGE! */

	ENTER("test_find");

	key = &keys[0];

	for (key = total_interval.from; key < total_interval.until; ++key) {
		status = red_black_lhmap_find(&lhmap,
					      (void *) key,
					      sizeof(*key));

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("lhmap_find",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("lhmap_find",
					     "KEY NOT FOUND: %d",
					     *key);
		}
	}


	unused_key = -1;

	status = red_black_lhmap_find(&lhmap,
				      (void *) &unused_key,
				      sizeof(unused_key));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("lhmap_find",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("lhmap_find",
				     "FOUND UNUSED KEY");
	}

	TEST_PASS("find");

	RETURN("test_find");

	return (ThreadReturn) 0;
}



ThreadReturn
test_iterator(void *arg)
{
	int *restrict key;
	unsigned int count;
	size_t length;
	int status;
	bool *restrict key_set_ptr;

	/* static bool key_set[KEYS_COUNT]; */

	RedBlackLHMapLIterator iterator;


	ENTER("test_iterator");

	/* calloc to preserve thread stack space */
	bool *const restrict key_set = calloc(KEYS_COUNT,
					      sizeof(*key_set));

	if (key_set == NULL)
		EXIT_ON_SYS_FAILURE("OUT OF MEMORY");

	/* test iterator */
	if (red_black_lhmap_literator_init(&iterator,
					   &lhmap) < 0)
				SYS_FAILURE("lhmap_literator_init",
					    "LOCK FAILURE");

	count = 0;

	while (1) {
		status = red_black_lhmap_literator_next(&iterator,
							(void **) &key,
							&length);

		if (status == 0)
			break;

		if (status < 0)
			SYS_FAILURE("lhmap_literator_next",
				    "LOCK FAILURE");

		if (length != sizeof(*key))
			TEST_FAILURE("lhmap_literator_next",
				     "GOT UNEXPECTED LENGTH %zu INSTEAD OF %zu",
				     length,
				     sizeof(*key));

		/* got next key */
		key_set_ptr = key_set + *key;

		if (*key_set_ptr)
			TEST_FAILURE("lhmap_literator_next",
				     "ITERATOR TRAVERSED SAME KEY TWICE: %d (%d) (%d)",
				     *key, *key_set_ptr, (int) (key - &keys[0]));

		*key_set_ptr = true;
		++count;
	}

	free(key_set);

	if (count < KEYS_COUNT)
		TEST_FAILURE("lhmap_literator",
			     "ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("lhmap_literator",
			     "ITERATOR TRAVERSED MORE THAN " KC_STR " KEYS");

	TEST_PASS("iterator");

	RETURN("test_iterator");

	return (ThreadReturn) 0;
}


ThreadReturn
test_delete(void *arg)
{
	int *restrict key;
	struct KeyInterval *restrict interval;
	int unused_key;
	int status;

	ENTER("test_delete");

	unused_key = -1;

	status = red_black_lhmap_delete(&lhmap,
					   (void *) &unused_key,
					   sizeof(unused_key));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("lhmap_delete",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("lhmap_delete",
				     "DELETED UNUSED KEY");
#if DO_VERIFY
	} else {
		status = red_black_lhmap_verify(&lhmap);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("lhmap_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("lhmap_delete",
					     "NOT VALID HASH MAP "
					     "(no deletion)");
		}
#endif /* if DO_VERIFY */
	}

	interval = (struct KeyInterval *) arg;

	key = &keys[0];

	for (key = interval->from; key < interval->until; ++key) {
		status = red_black_lhmap_delete(&lhmap,
						   (void *) key,
						   sizeof(*key));

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("lhmap_delete",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("lhmap_delete",
					     "KEY NOT FOUND: %d",
					     *key);
#if DO_VERIFY
		} else {
			status = red_black_lhmap_verify(&lhmap);

			if (status != 1) {
				if (status < 0)
					SYS_FAILURE("lhmap_verify",
						    "LOCK FAILURE");
				else
					TEST_FAILURE("lhmap_delete",
						     "NOT VALID HASH MAP "
						     "(deleted %d)",
						     *key);
			}
#endif /* if DO_VERIFY */
		}
	}


	status = red_black_lhmap_delete(&lhmap,
					   (void *) interval->from,
					   sizeof(*(interval->from)));

	if (status != 0) {
		if (status < 0)
			SYS_FAILURE("lhmap_delete",
				    "LOCK FAILURE");
		else
			TEST_FAILURE("lhmap_delete",
				     "DELETED KEY TWICE");
#if DO_VERIFY
	} else {
		status = red_black_lhmap_verify(&lhmap);

		if (status != 1) {
			if (status < 0)
				SYS_FAILURE("lhmap_verify",
					    "LOCK FAILURE");
			else
				TEST_FAILURE("lhmap_delete",
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

	/* test_count(0); */

	teardown();

	RETURN("test_single_thread_operations");
}

#include <unistd.h>
static inline void
multi_thread_test(const ThreadRoutine test)
{
	int i;
	Thread threads[THREADS_COUNT];

	ENTER("multi_thread_test");

	i = 0;

	do {
		if (!SPAWN_THREAD(threads[i],
				  test,
				  (void *) &intervals[i]))
			EXIT_ON_SYS_FAILURE("SPAWN_THREAD failure");


		++i;
	} while (i < THREADS_COUNT);

	i = 0;

	do {
		if (!WAIT_THREAD(threads[i]))
			EXIT_ON_SYS_FAILURE("WAIT_THREAD failure");

		++i;
	} while (i < THREADS_COUNT);

	RETURN("multi_thread_test");
}


static inline void
test_multi_thread_operations(void)
{
	ENTER("test_multi_thread_operations");

	setup();

	test_count(0);

	multi_thread_test(&test_insert);

	test_count(KEYS_COUNT);

	multi_thread_test(&test_find);

	test_count(KEYS_COUNT);

	multi_thread_test(&test_iterator);

	test_count(KEYS_COUNT);

	multi_thread_test(&test_delete);

	test_count(0);

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
