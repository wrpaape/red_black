#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_concat.h"  /* red_black_concat */
#include <stdio.h>             /* perror, printf */
#include <stdlib.h>            /* exit */
#include <stdint.h>	       /* intptr_t */


static RedBlackTree tree;

static int keys[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

#define KEY_COUNT (sizeof(keys) / sizeof(keys[0]))

static bool key_set[KEY_COUNT];


#define TEST_FAILURE(FAILURE)						\
do {									\
	fprintf(stderr,							\
		FAILURE "\n");						\
	red_black_tree_destroy(&tree);					\
	exit(1);							\
} while (0)
#define SYS_FAILURE(FAILURE)	\
do {									\
	perror(FAILURE);						\
	red_black_tree_destroy(&tree);					\
	exit(1);							\
} while (0)


int
main(void)
{
	int i;
	bool *restrict key_set_ptr;
	int key;

	struct RedBlackNode *restrict node;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	for (i = 0; i < KEY_COUNT; ++i)
		if (red_black_tree_insert(&tree,
					  (void *) (intptr_t) keys[i]) < 0)
			SYS_FAILURE("OUT OF MEMORY");


	node = tree.root;

	(void) red_black_concat(node,
				&node);

	while (node != NULL) {
		key = (int) (intptr_t) node->key;

		key_set_ptr = &key_set[key];

		if (*key_set_ptr)
			TEST_FAILURE("LOOP DETECTED");

		*key_set_ptr = true;

		node = node->left;
	}

	for (i = 0; i < KEY_COUNT; ++i)
		if (!key_set[i])
			TEST_FAILURE("INCOMPLETE LIST");

	puts("red_black_concat TEST PASSED");

	return 0;
}
