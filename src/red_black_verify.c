#include "red_black_verify.h"

int
do_red_black_verify(const struct RedBlackNode *const restrict node,
		    const RedBlackComparator comparator,
		    const void *const min_key,
		    const void *const max_key,
		    const bool parent_is_red,
		    int black_height)
{
	if (node == NULL)
		return black_height + 1;

	const void *const node_key = node->key;

	if (   (comparator(node_key,
			   min_key) < 0)
	    || (comparator(node_key,
			   max_key) > 0))
		return -1;

	const bool node_is_red = node->is_red;

	if (node_is_red) {
		if (parent_is_red)
			return -1;

	} else {
		++black_height;
	}

	const int left_black_height = do_red_black_verify(node->left,
							  min_key,
							  node_key,
							  node_is_red,
							  black_height);

	if (left_black_height < 0)
		return -1;

	const int right_black_height = do_red_black_verify(node->right,
							   node_key,
							   max_key,
							   node_is_red,
							   black_height);

	return (right_black_height == left_black_height)
	     ? right_black_height
	     : -1;
}


bool
red_black_verify(const struct RedBlackNode *const restrict root,
		 const RedBlackComparator comparator);
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
	const int left_black_height = do_red_black_verify(root->left,
							  comparator,
							  node->key,
							  root_key,
							  false,
							  0);

	if (left_black_height < 0)
		return false; /* invalid subtree */

	/* fetch max_key */
	node = root;

	while (1) {
		next = node->right;
		if (next == NULL)
			break; /* node is max leaf, node->key is max key */

		node = next;
	}

	/* check right black height */
	const int right_black_height = do_red_black_verify(root->right,
							   comparator,
							   root_key,
							   node->key,
							   false,
							   0);

	return (right_black_height == left_black_height);
}
