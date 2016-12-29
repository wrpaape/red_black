#include "red_black_verify.h"

int
do_red_black_verify(const struct RedBlackNode *const restrict node,
		    const struct Key *const restrict min_key,
		    const struct Key *const restrict max_key,
		    const bool parent_is_red,
		    int black_height)
{
	if (node == NULL)
		return black_height + 1;

	const struct Key *const restrict node_key = node->key;

	if (   (key_compare(node_key,
			    min_key) < 0)
	    || (key_compare(node_key,
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
red_black_verify(const struct RedBlackNode *const restrict tree)
{
	return (do_red_black_verify(tree,
				    &KEY_MIN,
				    &KEY_MAX,
				    true,
				    0) >= 0);
}
