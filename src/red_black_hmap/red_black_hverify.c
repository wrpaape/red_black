#include "red_black_hmap/red_black_hverify.h" /* HNode, cmptr, JUMP, NULL */

unsigned int
rb_hverify(const struct RedBlackHNode *const restrict node,
	   const struct RedBlackHKey *const restrict min_hkey,
	   const struct RedBlackHKey *const restrict max_hkey,
	   const bool parent_is_red,
	   unsigned int black_height,
	   RedBlackJumpBuffer jump_buffer)
{
	if (node == NULL)
		return black_height + 1;

	const struct RedBlackHKey *const restrict node_hkey = &node->hkey;

	if (   (red_black_hkey_comparator(node_hkey,
					  min_hkey) < 0)
	    || (red_black_hkey_comparator(node_hkey,
					  max_hkey) > 0))
		RED_BLACK_JUMP_1(jump_buffer); /* keys out of order */

	const bool node_is_red = node->is_red;

	if (node_is_red) {
		if (parent_is_red)
			RED_BLACK_JUMP_1(jump_buffer); /* consecutive red */

	} else {
		++black_height;
	}

	const unsigned int left_black_height = rb_hverify(node->left,
							  min_hkey,
							  node_hkey,
							  node_is_red,
							  black_height,
							  jump_buffer);


	const unsigned int right_black_height = rb_hverify(node->right,
							   node_hkey,
							   max_hkey,
							   node_is_red,
							   black_height,
							   jump_buffer);

	if (left_black_height != right_black_height)
		RED_BLACK_JUMP_1(jump_buffer); /* tree imbalanced */

	return left_black_height;
}


void
red_black_hverify(const struct RedBlackHNode *const restrict root,
		  RedBlackJumpBuffer jump_buffer)
{

	(void) rb_hverify(root,
			  &RED_BLACK_HKEY_MIN,
			  &RED_BLACK_HKEY_MAX,
			  false,
			  0,
			  jump_buffer); /* valid if didn't jump */
}
