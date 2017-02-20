#include "unity.h"                            /* UNITY testing framework */
#include "red_black_tree/red_black_treeify.h" /* red_black_treeify, Node */
#include "red_black_tree/red_black_verify.h"  /* red_black_verify, JumpBuffer */
#include "int_key.h"                          /* int_key_comparator */
#include "red_black_test.h"		      /* KEY_COUNT */
#include <stdint.h>			      /* intptr_t */


static struct RedBlackNode nodes[KEYS_COUNT];

void
setUp(void)
{
}

void
tearDown(void)
{
}


void
test_red_black_treeify(void)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	RedBlackJumpBuffer jump_buffer;
	bool status;

	node = &nodes[0];
	head = NULL;

	for (int i = KEYS_COUNT - 1; i >= 0; --i) {
		node->key   = (void *) (intptr_t) i;
		node->right = head;

		head = node;

		++node;
	}

	node = red_black_treeify(head,
				 KEYS_COUNT);

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0)
	      && red_black_verify(node,
				  &int_key_comparator,
				  jump_buffer);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "INVALID TREE");
}
