#include "red_black_flatten.h"	   /* struct RedBlackNode */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */

struct RedBlackNode *restrict *restrict
rb_concat_rtree(struct RedBlackNode *restrict node,
		struct RedBlackNode *restrict *const restrict end_ptr)
{
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;
	struct RedBlackNode *restrict *restrict end_ptr;
	struct RedBlackNode *restrict next;

	if (node == NULL)
		return end_ptr;

	*end_ptr = node;

	cursor  = &stack[0];
	*cursor = NULL;

	while (1) {
		next = node->left;

		if (next == NULL)
			break;

		++cursor;
		*cursor = node;

		node = next;
	}

	end_ptr = &node->left;

	while (1) {
		end_ptr = rb_concat_rtree(node->right,
					  end_ptr);

		node = *cursor;

		if (node == NULL)
			return end_ptr;

		--cursor;
	}

}

void
red_black_flatten(struct RedBlackNode *restrict node)
{
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;
	struct RedBlackNode *restrict *restrict end_ptr;
	struct RedBlackNode *restrict next;

	if (node == NULL)
		return;

	cursor  = &stack[0];
	*cursor = NULL;

	while (1) {
		next = node->left;

		if (next == NULL)
			break;

		++cursor;
		*cursor = node;

		node = next;
	}

	end_ptr = &node->left;

	while (1) {
		end_ptr = rb_concat_rtree(node->right,
					  end_ptr);

		node = *cursor;

		if (node == NULL)
			return;

		--cursor;
	}
}
