#include "red_black_test.h"     /* print/exit macros */
#include "red_black_mt_test.h"  /* multi_thread_test */
#include "red_black_lhmap.h"    /* red_lhmap_insert|find|verify|delete */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackLHMap lhmap;


static inline void
setup(void)
{
	ENTER("setup");

	init_keys();

	seed_random();

	shuffle_keys();

	if (!red_black_lhmap_init(&lhmap))
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
test_itor(void *arg)
{
	int *restrict key;
	unsigned int count;
	size_t length;
	int status;
	bool *restrict key_set_ptr;

	/* static bool key_set[KEYS_COUNT]; */

	RedBlackLHMapLItor itor;


	ENTER("test_itor");

	/* calloc to preserve thread stack space */
	bool *const restrict key_set = calloc(KEYS_COUNT,
					      sizeof(*key_set));

	if (key_set == NULL)
		EXIT_ON_SYS_FAILURE("OUT OF MEMORY");

	/* test itor */
	if (!red_black_lhmap_litor_init(&itor,
					&lhmap))
				SYS_FAILURE("lhmap_litor_init",
					    "LOCK FAILURE");

	count = 0;

	while (1) {
		status = red_black_lhmap_litor_next(&itor,
						    (void **) &key,
						    &length);

		if (status == 0)
			break;

		if (status < 0)
			SYS_FAILURE("lhmap_litor_next",
				    "LOCK FAILURE");

		if (length != sizeof(*key))
			TEST_FAILURE("lhmap_litor_next",
				     "GOT UNEXPECTED LENGTH %zu INSTEAD OF %zu",
				     length,
				     sizeof(*key));

		/* got next key */
		key_set_ptr = key_set + *key;

		if (*key_set_ptr)
			TEST_FAILURE("lhmap_litor_next",
				     "ITERATOR TRAVERSED SAME KEY TWICE: %d (%d) (%d)",
				     *key, *key_set_ptr, (int) (key - &keys[0]));

		*key_set_ptr = true;
		++count;
	}

	free(key_set);

	if (count < KEYS_COUNT)
		TEST_FAILURE("lhmap_litor",
			     "ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("lhmap_litor",
			     "ITERATOR TRAVERSED MORE THAN " KC_STR " KEYS");

	TEST_PASS("itor");

	RETURN("test_itor");

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

	(void) test_itor(NULL);

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

	multi_thread_test(&test_insert);

	test_count(KEYS_COUNT);

	multi_thread_test(&test_find);

	test_count(KEYS_COUNT);

	multi_thread_test(&test_itor);

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
