#include "red_black_set.h" /* Node, Comparator */

void
red_black_set(struct RedBlackNode *restrict node,
	      const RedBlackComparator comparator,
	      const void *const key)
{
	int compare;

	while (1) {
		compare = comparator(key,
				     node->key);

		if (compare == 0) {
			node->key = key;
			return;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}
