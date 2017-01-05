#include "red_black_fetch.h"
#include <stddef.h>	/* NULL */

bool
red_black_fetch(const struct RedBlackNode *restrict node,
		const RedBlackComparator comparator,
		const void *const key,
		void **const restrict key_ptr)
{
	int compare;
	const void *node_key;

	while (node != NULL) {
		node_key = node->key;

		compare = comparator(key,
				     node_key);

		if (compare == 0) {
			*key_ptr = (void *) node_key;
			return true;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}

	return false;
}
