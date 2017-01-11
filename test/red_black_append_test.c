#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_append.h"  /* red_black_append */
#include <stdio.h>             /* fprintf */
#include <stdlib.h>            /* exit */


static RedBlackTree tree;


static int keys[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

#define KEY_COUNT (sizeof(keys) / sizeof(keys[0]))

static bool key_set[KEY_COUNT];
static struct RedBlackNode nodes[KEY_COUNT];


#define TEST_FAILURE(FAILURE)						\
do {									\
	fprintf(stderr,							\
		FAILURE "\n");						\
	red_black_tree_destroy(&tree);					\
	exit(1);							\
} while (0)

int
main(void)
{
	bool *restrict key_set_ptr;
	int key;
	struct RedBlackNode *restrict node;
	RedBlackTreeIterator iterator;
	RedBlackJumpBuffer jump_buffer;

	red_black_tree_init(&tree,
			    &int_key_comparator);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
		goto CONTINUE_APPENDING;

	for (volatile int i = 0; i < KEY_COUNT; ++i) {
		node = &nodes[i];

		node->key = (const void *) (intptr_t) keys[i];

		red_black_append(&tree.root,
				 tree.comparator,
				 &jump_buffer,
				 node);
CONTINUE_APPENDING:
		continue;
	}

	red_black_tree_iterator_init_asc(&iterator,
					 &tree);

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &key)) {
		key_set_ptr = &key_set[key];

		if (*key_set_ptr)
			TEST_FAILURE("KEY REPEATED");

		*key_set_ptr = true;
	}

	for (int i = 0; i < KEY_COUNT; ++i)
		if (!key_set[i])
			TEST_FAILURE("INCOMPLETE TREE");

	puts("red_black_append TEST PASSED");

	return 0;
}
