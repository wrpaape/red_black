#include "red_black_tree/red_black_replace.h" /* HNode, Comparator */
#include <stddef.h>			      /* NULL */

bool
red_black_replace(struct RedBlackHNode *restrict node,
		  const RedBlackComparator comparator,
		  const void *const key)
{
	int compare;
	const void *node_key;

	while (node != NULL) {
		node_key = node->key;

		compare = comparator(key,
				     node_key);

		if (compare == 0) {
			node->key = key;
			return true;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}

	return false;
}


bool
red_black_replace_min(struct RedBlackHNode *restrict node,
		      const void *const key)
{
	struct RedBlackHNode *restrict next;

	const bool nodes_present = (node != NULL);

	if (nodes_present) {
		while (1) {
			next = node->left;
			if (next == NULL)
				break;

			node = next;
		}

		node->key = key;
	}

	return nodes_present;
}


bool
red_black_replace_max(struct RedBlackHNode *restrict node,
		      const void *const key)
{
	struct RedBlackHNode *restrict next;

	const bool nodes_present = (node != NULL);

	if (nodes_present) {
		while (1) {
			next = node->right;
			if (next == NULL)
				break;

			node = next;
		}

		node->key = key;
	}

	return nodes_present;
}
