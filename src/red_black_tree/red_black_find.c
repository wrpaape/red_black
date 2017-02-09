#include "red_black_tree/red_black_find.h" /* Node, Comparator */
#include <stddef.h>			   /* NULL */


bool
red_black_find(const struct RedBlackNode *restrict node,
	       const RedBlackComparator comparator,
	       const void *const key)
{
	int compare;

	while (node != NULL) {
		compare = comparator(key,
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
