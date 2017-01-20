#include "red_black_fetch.h" /* Node, Comparator */
#include <stddef.h>	     /* NULL */

bool
red_black_fetch(const struct RedBlackNode *restrict node,
		const RedBlackComparator comparator,
		const void *const key,
		void **const restrict fetch_ptr)
{
	int compare;
	const void *node_key;

	while (node != NULL) {
		node_key = node->key;

		compare = comparator(key,
				     node_key);

		if (compare == 0) {
			*fetch_ptr = (void *) node_key;
			return true;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}

	return false;
}


bool
red_black_fetch_min(const struct RedBlackNode *restrict node,
		    void **const restrict fetch_ptr)
{
	const struct RedBlackNode *restrict next;

	const bool nodes_present = (node != NULL);

	if (nodes_present) {
		while (1) {
			next = node->left;
			if (next == NULL)
				break;

			node = next;
		}

		*fetch_ptr = (void *) node->key;
	}

	return nodes_present;
}


bool
red_black_fetch_max(const struct RedBlackNode *restrict node,
		    void **const restrict fetch_ptr)
{
	const struct RedBlackNode *restrict next;

	const bool nodes_present = (node != NULL);

	if (nodes_present) {
		while (1) {
			next = node->right;
			if (next == NULL)
				break;

			node = next;
		}

		*fetch_ptr = (void *) node->key;
	}

	return nodes_present;
}
