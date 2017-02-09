#include "red_black_tree/red_black_swap.h" /* Node, Comparator */
#include <stddef.h>			   /* NULL */

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


void *
red_black_swap_min(struct RedBlackNode *restrict node,
		   const void *const key)
{
	struct RedBlackNode *restrict next;
	const void *node_key;

	while (1) {
		next = node->left;
		if (next == NULL) {
			node_key  = node->key;
			node->key = key;
			return (void *) node_key;
		}

		node = next;
	}
}


void *
red_black_swap_max(struct RedBlackNode *restrict node,
		   const void *const key)
{
	struct RedBlackNode *restrict next;
	const void *node_key;

	while (1) {
		next = node->right;
		if (next == NULL) {
			node_key  = node->key;
			node->key = key;
			return (void *) node_key;
		}

		node = next;
	}
}
