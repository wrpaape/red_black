#include "red_black_test.h"	/* deps, print macros */

#define KEYS_COUNT 5
#define I_LAST     (KEYS_COUNT - 1)

static struct RedBlackTree tree;
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
	unsigned int rand;

	const unsigned int span      = upto + 1;
	const unsigned int threshold = -span % span;

	do {
		rand = (unsigned int) random();
	} while (rand < threshold);

	return rand % span;
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
	unsigned int i_old;
	unsigned int i_new;

	ENTER("shuffle");

	i_old = 0;

	do {
		i_new = i_old + random_upto(I_LAST - i_old);

		swap(&keys[i_old],
		     &keys[i_new]);

		++i_old;
	} while (i_old < I_LAST);

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

	shuffle();

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
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("insert",
				     "NOT VALID TREE (inserted %d)",
				     *key);

		++key;
	} while (key < keys_until);

	TEST_PASS("insert");

	RETURN("test_insert");
}

static inline void
test_find(void)
{
	int *restrict key;

	ENTER("test_find");

	key = &keys[0];

	do {
		if (!red_black_tree_find(&tree,
					 (void *) (intptr_t) *key))
			TEST_FAILURE("find",
				     "KEY NOT FOUND: %d",
				     *key);

		++key;
	} while (key < keys_until);

	TEST_PASS("find");

	RETURN("test_find");
}

static inline void
test_delete(void)
{
	int *restrict key;

	ENTER("test_delete");

	key = &keys[0];

	do {
		printf("deleting key: %d\n", *key);
		fflush(stdout);

		PRINT_TREE();

		if (!red_black_tree_delete(&tree,
					   (void *) (intptr_t) *key))
			TEST_FAILURE("delete",
				     "KEY NOT FOUND: %d",
				     *key);
		else if (!red_black_tree_verify(&tree))
			TEST_FAILURE("insert",
				     "NOT VALID TREE (deleted %d)",
				     *key);

		printf("deleted key: %d\n", *key);
		fflush(stdout);
		++key;
	} while (key < keys_until);

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

	shuffle();

	test_find();

	shuffle();

	test_delete();

	teardown();

	return 0;
}
