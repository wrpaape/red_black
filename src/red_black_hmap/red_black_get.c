#include "red_black_tree/red_black_get.h" /* HNode, Comparator */
#include <stddef.h>			  /* NULL */

void *
red_black_get(const struct RedBlackHNode *restrict node,
	      const RedBlackComparator comparator,
	      const void *const key)
{
	int compare;
	const void *node_key;

	while (1) {
		node_key = node->key;

		compare = comparator(key,
				     node_key);

		if (compare == 0)
			return (void *) node_key;

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}


void *
red_black_get_min(const struct RedBlackHNode *restrict node)
{
	const struct RedBlackHNode *restrict next;

	while (1) {
		next = node->left;
		if (next == NULL)
			return (void *) node->key;

		node = next;
	}
}


void *
red_black_get_max(const struct RedBlackHNode *restrict node)
{
	const struct RedBlackHNode *restrict next;

	while (1) {
		next = node->right;
		if (next == NULL)
			return (void *) node->key;

		node = next;
	}
}
