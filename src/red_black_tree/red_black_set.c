#include "red_black_tree/red_black_set.h" /* Node, Comparator */
#include <stddef.h>			  /* NULL */

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


void
red_black_set_min(struct RedBlackNode *restrict node,
		  const void *const key)
{
	struct RedBlackNode *restrict next;

	while (1) {
		next = node->left;
		if (next == NULL) {
			node->key = key;
			return;
		}

		node = next;
	}
}


void
red_black_set_max(struct RedBlackNode *restrict node,
		  const void *const key)
{
	struct RedBlackNode *restrict next;

	while (1) {
		next = node->right;
		if (next == NULL) {
			node->key = key;
			return;
		}

		node = next;
	}
}
