#include "red_black_tree/red_black_verify.h" /* Node, Comparator, JUMP API */
#include <stddef.h>			     /* NULL */

unsigned int
rb_verify(const struct RedBlackNode *const restrict node,
	  const RedBlackComparator comparator,
	  const void *const min_key,
	  const void *const max_key,
	  const bool parent_is_red,
	  unsigned int black_height,
	  RedBlackJumpBuffer jump_buffer)
{
	if (node == NULL)
		return black_height + 1;

	const void *const node_key = node->key;

	if (   (comparator(node_key,
			   min_key) < 0)
	    || (comparator(node_key,
			   max_key) > 0))
		RED_BLACK_JUMP_1(jump_buffer); /* keys out of order */

	const bool node_is_red = node->is_red;

	if (node_is_red) {
		if (parent_is_red)
			RED_BLACK_JUMP_1(jump_buffer); /* consecutive red */

	} else {
		++black_height;
	}

	const unsigned int left_black_height = rb_verify(node->left,
							 comparator,
							 min_key,
							 node_key,
							 node_is_red,
							 black_height,
							 jump_buffer);


	const unsigned int right_black_height = rb_verify(node->right,
							  comparator,
							  node_key,
							  max_key,
							  node_is_red,
							  black_height,
							  jump_buffer);

	if (left_black_height != right_black_height)
		RED_BLACK_JUMP_1(jump_buffer); /* tree imbalanced */

	return left_black_height;
}


bool
red_black_verify(const struct RedBlackNode *const restrict root,
		 const RedBlackComparator comparator,
		 RedBlackJumpBuffer jump_buffer)
{
	const struct RedBlackNode *restrict node;
	const struct RedBlackNode *restrict next;

	if (root == NULL)
		return true; /* valid tree */

	/* fetch min_key */
	node = root;

	while (1) {
		next = node->left;
		if (next == NULL)
			break; /* node is min leaf, node->key is min key */

		node = next;
	}


	const void *const root_key = root->key;

	/* check left black height */
	const unsigned int left_black_height = rb_verify(root->left,
							 comparator,
							 node->key,
							 root_key,
							 false,
							 0,
							 jump_buffer);

	/* fetch max_key */
	node = root;

	while (1) {
		next = node->right;
		if (next == NULL)
			break; /* node is max leaf, node->key is max key */

		node = next;
	}

	/* check right black height */
	const unsigned int right_black_height = rb_verify(root->right,
							  comparator,
							  root_key,
							  node->key,
							  false,
							  0,
							  jump_buffer);

	/* if returned both left and right subtrees are valid red black trees,
	 * final check for balance */
	return (left_black_height == right_black_height);
}
