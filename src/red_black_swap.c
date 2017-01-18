#include "red_black_swap.h" /* Node, Comparator */

void *
red_black_swap(struct RedBlackNode *restrict node,
	       const RedBlackComparator comparator,
	       const void *const key)
{
	int compare;
	const void *node_key;

	while (1) {
		node_key = node->key;

		compare = comparator(key,
				     node_key);

		if (compare == 0) {
			node->key = key;
			return (void *) node_key;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}
