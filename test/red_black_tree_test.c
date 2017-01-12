#include "red_black_test.h" /* print macros, keys */
#include "red_black_tree.h" /* red_black_insert|find|verify|delete */
#include "int_key.h"        /* int_key_comparator */
#include <signal.h>         /* sigaction */





/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackTree tree;

void
handle_segfault(int signo)
{
	WRITE_LITERAL("\ncaught SIGSEGV\n");

	if (red_black_tree_print(&tree,
				 &int_key_sizer,
				 &int_key_putter))
		EXIT_ON_SYS_FAILURE("segfault");
	else
		EXIT_ON_SYS_FAILURE("segfault and write/OOM");
}

static inline void
setup(void)
{
	struct sigaction action;

	ENTER("setup");

	action.sa_handler = &handle_segfault;

	sigemptyset(&action.sa_mask);

	action.sa_flags = 0;

	/* attempt to print tree on segfault */
	if (sigaction(SIGSEGV,
		      &action,
		      NULL) < 0)
		EXIT_ON_SYS_FAILURE("sigaction failure");

	seed_random();

	init_keys();

	red_black_tree_init(&tree,
			    &int_key_comparator);

	RETURN("setup");
}

static inline void
test_insert(void)
{
	int *restrict key;
	int status;

	ENTER("test_insert");

	shuffle_keys();

	key = &keys[0];

	do {
		status = red_black_tree_insert(&tree,
					       (void *) (intptr_t) *key);

		if (status < 0)
			SYS_FAILURE("tree_insert",
				    "OUT OF MEMORY");
		else if (status == 0)
			TEST_FAILURE("tree_insert",
				     "KEY NOT UNIQUE: %d",
				     *key);
#if DO_VERIFY
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("tree_insert",
				     "NOT VALID TREE (inserted %d)",
				     *key);
#endif /* if DO_VERIFY */

		++key;
	} while (key < keys_until);

	status = red_black_tree_insert(&tree,
				       (void *) (intptr_t) 0);

	if (status < 0)
		SYS_FAILURE("tree_insert",
			    "OUT OF MEMORY");
	else if (status == 1)
		TEST_FAILURE("tree_insert",
			     "KEY INSERTED TWICE");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("tree_insert",
			     "NOT VALID TREE (no insertion)");
#endif /* if DO_VERIFY */

	TEST_PASS("insert");

	RETURN("test_insert");
}

static inline void
test_find(void)
{
	int *restrict key;

	ENTER("test_find");

	shuffle_keys();

	key = &keys[0];

	do {
		if (!red_black_tree_find(&tree,
					 (void *) (intptr_t) *key))
			TEST_FAILURE("tree_find",
				     "KEY NOT FOUND: %d",
				     *key);

		++key;
	} while (key < keys_until);

	if (red_black_tree_find(&tree,
				(void *) (intptr_t) -1))
		TEST_FAILURE("tree_find",
			     "FOUND UNUSED KEY");

	TEST_PASS("find");

	RETURN("test_find");
}

static inline void
test_iterator(void)
{
	int last_key;
	int next_key;
	unsigned int count;

	RedBlackTreeIterator iterator;

	ENTER("test_iterator");

	/* test ascending iterator */

	red_black_tree_iterator_init_asc(&iterator,
					 &tree);

	last_key = -1;
	count    = 0;

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &next_key)) {
		if (last_key > next_key)
			TEST_FAILURE("tree_iterator",
				     "ASCENDING ITERATOR OUT OF ORDER");

		last_key = next_key;

		++count;
	}

	if (count < KEYS_COUNT)
		TEST_FAILURE("tree_iterator",
			     "ASCENDING ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("tree_iterator",
			     "ASCENDING ITERATOR REPEATED ENTRIES");

	/* test descending iterator */

	red_black_tree_iterator_init_desc(&iterator,
					  &tree);

	last_key = KEYS_COUNT + 1;
	count    = 0;

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &next_key)) {
		if (last_key < next_key)
			TEST_FAILURE("tree_iterator",
				     "DESCENDING ITERATOR OUT OF ORDER");

		last_key = next_key;

		++count;
	}

	if (count < KEYS_COUNT)
		TEST_FAILURE("tree_iterator",
			     "DESCENDING ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("tree_iterator",
			     "DESCENDING ITERATOR REPEATED ENTRIES");

	TEST_PASS("iterator");

	RETURN("test_iterator");
}


static inline void
test_count(const unsigned int expected)
{
	ENTER("test_count");

	const unsigned int count = red_black_tree_count(&tree);

	if (count < expected)
		TEST_FAILURE("tree_count",
			     "COUNT LESS THAN EXPECTED: %u < %u",
			     count,
			     expected);
	else if (count > expected)
		TEST_FAILURE("tree_count",
			     "COUNT GREATER THAN EXPECTED: %u > %u",
			     count,
			     expected);

	TEST_PASS("count");

	RETURN("test_count");
}

static inline void
test_delete(void)
{
	int *restrict key;

	ENTER("test_delete");

	shuffle_keys();

	if (red_black_tree_delete(&tree,
				  (void *) (intptr_t) -1))
		TEST_FAILURE("tree_delete",
			     "DELETED UNUSED KEY");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("tree_delete",
			     "NOT VALID TREE (no deletion)");
#endif /* if DO_VERIFY */

	key = &keys[0];

	do {
		if (!red_black_tree_delete(&tree,
					   (void *) (intptr_t) *key))
			TEST_FAILURE("tree_delete",
				     "KEY NOT FOUND: %d",
				     *key);
#if DO_VERIFY
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("tree_delete",
				     "NOT VALID TREE (deleted %d)",
				     *key);
#endif /* if DO_VERIFY */

		++key;
	} while (key < keys_until);

	if (red_black_tree_delete(&tree,
				  (void *) (intptr_t) keys[0]))
		TEST_FAILURE("tree_delete",
			     "DELETED KEY TWICE");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("tree_delete",
			     "NOT VALID TREE (no deletion)");
#endif /* if DO_VERIFY */

	TEST_PASS("delete");

	RETURN("test_delete");
}

static inline void
teardown(void)
{
	ENTER("teardown");

	red_black_tree_destroy(&tree);

	RETURN("teardown");
}



int
main(void)
{
	setup();

	STARTING_TESTS();

	test_count(0);

	test_insert();

	test_count(KEYS_COUNT);

	test_find();

	test_count(KEYS_COUNT);

	test_iterator();

	test_count(KEYS_COUNT);

	test_delete();

	test_count(0);

	ALL_TESTS_PASSED();

	teardown();

	return 0;
}
