#include "red_black_test.h"	/* deps, print macros */

#define DO_VERIFY 0

#define KEYS_COUNT 1000000

#define I_LAST     (KEYS_COUNT - 1)
#define STR(X)  #X
#define XSTR(X) STR(X)
#define KC_STR  XSTR(KEYS_COUNT)

static RedBlackTree tree;
static int keys[KEYS_COUNT];
static const int *const restrict keys_until = &keys[KEYS_COUNT];

void
handle_segfault(int signo)
{
	if (PRINT_TREE())
		EXIT_ON_SYS_FAILURE("segfault");
	else
		EXIT_ON_SYS_FAILURE("segfault and write/OOM");
}

static inline unsigned int
random_upto(const unsigned int upto)
{
	unsigned long rand;

	const unsigned long span      = (unsigned long) (upto + 1);
	const unsigned long threshold = -span % span;

	do {
		rand = (unsigned long) random();
	} while (rand < threshold);

	return (unsigned int) (rand % span);
}

static inline void
swap(int *const restrict x,
     int *const restrict y)
{
	const int tmp = *x;
	*x = *y;
	*y = tmp;
}

static inline void
shuffle(void)
{
	int i_old;
	int i_new;

	ENTER("shuffle");

	i_old = 0;

	for (i_old = 0; i_old < I_LAST; ++i_old) {
		i_new = i_old + random_upto(I_LAST - i_old);

		swap(&keys[i_old],
		     &keys[i_new]);
	}

	RETURN("shuffle");
}

static inline void
setup(void)
{
	int i;
	struct sigaction action;

	ENTER("setup");

	action.sa_handler = &handle_segfault;

	sigemptyset(&action.sa_mask);

	action.sa_flags = 0;

	if (sigaction(SIGSEGV,
		      &action,
		      NULL) < 0)
		EXIT_ON_SYS_FAILURE("sigaction failure");

	i = 0;

	do {
		keys[i] = i;
		++i;
	} while (i < KEYS_COUNT);

	srandom((unsigned) time(NULL));

	red_black_tree_init(&tree,
			    &red_black_int_key_comparator);

	RETURN("setup");
}

static inline void
test_insert(void)
{
	int *restrict key;
	int status;

	ENTER("test_insert");

	shuffle();

	key = &keys[0];

	do {
		status = red_black_tree_insert(&tree,
					       (void *) (intptr_t) *key);

		if (status < 0)
			SYS_FAILURE("insert",
				    "OUT OF MEMORY");
		else if (status == 0)
			TEST_FAILURE("insert",
				     "KEY NOT UNIQUE: %d",
				     *key);
#if DO_VERIFY
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("insert",
				     "NOT VALID TREE (inserted %d)",
				     *key);
#endif /* if DO_VERIFY */

		++key;
	} while (key < keys_until);

	status = red_black_tree_insert(&tree,
				       (void *) (intptr_t) 0);

	if (status < 0)
		SYS_FAILURE("insert",
			    "OUT OF MEMORY");
	else if (status == 1)
		TEST_FAILURE("insert",
			     "'0' INSERTED TWICE");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("insert",
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

	shuffle();

	key = &keys[0];

	do {
		if (!red_black_tree_find(&tree,
					 (void *) (intptr_t) *key))
			TEST_FAILURE("find",
				     "KEY NOT FOUND: %d",
				     *key);

		++key;
	} while (key < keys_until);

	if (red_black_tree_find(&tree,
				(void *) (intptr_t) -1))
		TEST_FAILURE("find",
			     "FOUND NEGATIVE KEY");

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
					    (const void **) &next_key)) {
		if (last_key > next_key)
			TEST_FAILURE("iterator",
				     "ASCENDING ITERATOR OUT OF ORDER");

		last_key = next_key;

		++count;
	}

	if (count < KEYS_COUNT)
		TEST_FAILURE("iterator",
			     "ASCENDING ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("iterator",
			     "ASCENDING ITERATOR REPEATED ENTRIES");

	/* test descending iterator */

	red_black_tree_iterator_init_desc(&iterator,
					  &tree);

	last_key = KEYS_COUNT + 1;
	count    = 0;

	while (red_black_tree_iterator_next(&iterator,
					    (const void **) &next_key)) {
		if (last_key < next_key)
			TEST_FAILURE("iterator",
				     "DESCENDING ITERATOR OUT OF ORDER");

		last_key = next_key;

		++count;
	}

	if (count < KEYS_COUNT)
		TEST_FAILURE("iterator",
			     "DESCENDING ITERATOR SKIPPED ENTRIES");
	else if (count > KEYS_COUNT)
		TEST_FAILURE("iterator",
			     "DESCENDING ITERATOR REPEATED ENTRIES");

	TEST_PASS("iterator");

	RETURN("test_iterator");
}


static inline void
test_count(void)
{
	ENTER("test_count");

	const unsigned int count = red_black_tree_count(&tree);

	if (count < KEYS_COUNT)
		TEST_FAILURE("count",
			     "COUNT LESS THAN EXPECTED: %d < " KC_STR,
			     count);
	else if (count > KEYS_COUNT)
		TEST_FAILURE("count",
			     "COUNT GREATER THAN EXPECTED: %d > " KC_STR,
			     count);

	TEST_PASS("count");

	RETURN("test_count");
}

static inline void
test_delete(void)
{
	int *restrict key;

	ENTER("test_delete");

	shuffle();

	if (red_black_tree_delete(&tree,
				  (void *) (intptr_t) -1))
		TEST_FAILURE("delete",
			     "DELETED NEGATIVE KEY");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("delete",
			     "NOT VALID TREE (no deletion)");
#endif /* if DO_VERIFY */

	key = &keys[0];

	do {
		if (!red_black_tree_delete(&tree,
					   (void *) (intptr_t) *key))
			TEST_FAILURE("delete",
				     "KEY NOT FOUND: %d",
				     *key);
#if DO_VERIFY
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("delete",
				     "NOT VALID TREE (deleted %d)",
				     *key);
#endif /* if DO_VERIFY */

		++key;
	} while (key < keys_until);

	if (red_black_tree_delete(&tree,
				  (void *) (intptr_t) 0))
		TEST_FAILURE("delete",
			     "DELETED 0 TWICE");
#if DO_VERIFY
	else if (!red_black_tree_verify(&tree))
		TEST_FAILURE("delete",
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

	test_insert();

	test_find();

	test_iterator();

	test_count();

	test_delete();

	teardown();

	return 0;
}
