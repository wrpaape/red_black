#include "red_black_hmap/red_black_hconcat.h"	    /* RedBlackHNode, NULL */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */

struct RedBlackHNode *restrict *restrict
red_black_hconcat(struct RedBlackHNode *restrict node,
		  struct RedBlackHNode *restrict *restrict end_ptr)
{
	struct RedBlackHNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackHNode *restrict *restrict cursor;
	struct RedBlackHNode *restrict next;

	if (node == NULL)
		return end_ptr;

	*end_ptr = node; /* concat with root of right subtree */

	cursor  = &stack[0];
	*cursor = NULL;

	/* fetch next end_ptr */
	while (1) {
		next = node->left;

		if (next == NULL)
			break;

		++cursor;
		*cursor = node;

		node = next;
	}

	end_ptr = &node->left;

	/* unwind stack, concatting right subtrees along the way */
	while (1) {
		end_ptr = red_black_hconcat(node->right,
					    end_ptr);

		node = *cursor;

		if (node == NULL)
			return end_ptr;

		--cursor;
	}
}
