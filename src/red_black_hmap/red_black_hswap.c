#include "red_black_hmap/red_black_hswap.h" /* HNode, comparator, NULL */

void *
red_black_swap(struct RedBlackHNode *restrict node,
	       const struct RedBlackHKey *const restrict hkey)
{
	int compare;
	const void *node_key;

	while (1) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			node_key       = node->hkey.key;
			node->hkey.key = hkey->key;
			return (void *) node_key;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}
