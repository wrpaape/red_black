#include "red_black_find.h"

bool
red_black_find(const struct RedBlackNode *restrict node,
	       const struct Key *const restrict key)
{
	int64_t compare;

	while (node != NULL) {
		compare = key_compare(key,
				      node->key);

		if (compare < 0)
			node = node->left;
		else if (compare > 0)
			node = node->right;
		else
			return true;
	}

	return false;
}
