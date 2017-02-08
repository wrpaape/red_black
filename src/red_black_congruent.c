#include "red_black_congruent.h" /* RedBlackNode */
#include <stddef.h>		 /* NULL */

void
red_black_congruent(const struct RedBlackNode *const restrict node1,
		    const struct RedBlackNode *const restrict node2,
		    const RedBlackComparator comparator,
		    RedBlackJumpBuffer jump_buffer)
{
	if (node1 == NULL) {
		if (node2 == NULL)
			return;
		else
			RED_BLACK_JUMP_1(jump_buffer);
	}

	if (   (node2 == NULL)
	    || (comparator(node1->key,
			   node2->key) != 0))
		RED_BLACK_JUMP_1(jump_buffer);


	red_black_congruent(node1->left,
			    node2->left,
			    comparator,
			    jump_buffer);

	red_black_congruent(node1->right,
			    node2->right,
			    comparator,
			    jump_buffer);
}
