#include "red_black_hmap/red_black_hcongruent.h" /* HNode, comparator, JUMP */


void
red_black_hcongruent(const struct RedBlackHNode *const restrict node1,
		     const struct RedBlackHNode *const restrict node2,
		     RedBlackJumpBuffer jump_buffer)
{
	if (node1 == NULL) {
		if (node2 == NULL)
			return;
		else
			RED_BLACK_JUMP_1(jump_buffer);
	}

	if (   (node2 == NULL)
	    || (red_black_hkey_comparator(&node1->hkey,
					  &node2->hkey) != 0))
		RED_BLACK_JUMP_1(jump_buffer);


	red_black_hcongruent(node1->left,
			     node2->left,
			     jump_buffer);

	red_black_hcongruent(node1->right,
			     node2->right,
			     jump_buffer);
}
