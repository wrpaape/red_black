#include "red_black_concat.h"	   /* struct RedBlackNode */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		   /* NULL */

struct RedBlackNode *restrict *restrict
red_black_concat(struct RedBlackNode *restrict node,
		 struct RedBlackNode *restrict *restrict end_ptr)
{
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;
	struct RedBlackNode *restrict next;

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
		end_ptr = red_black_concat(node->right,
					   end_ptr);

		node = *cursor;

		if (node == NULL)
			return end_ptr;

		--cursor;
	}

}
