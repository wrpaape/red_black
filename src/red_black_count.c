#include "red_black_count.h"
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */
#include <stddef.h>                /* NULL */

unsigned int
red_black_count(const struct RedBlackNode *restrict node)
{
	const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	const struct RedBlackNode *restrict *restrict cursor;
	const struct RedBlackNode *restrict next;
	unsigned int count;

	if (node == NULL)
		return 0;

	count = 0;

	cursor  = &stack[0];
	*cursor = NULL;

	while (1) {
		while (1) { /* fetch minimum */
			next = node->left;

			if (next == NULL)
				break;

			++cursor;
			*cursor = node;

			node = next;
		}

		while (1) { /* find next root */
			++count; /* count root */

			node = node->right; /* set next root */

			if (node != NULL)
				break;

			node = *cursor; /* unwind stack */

			if (node == NULL)
				return count;

			--cursor;
		}
	}
}
