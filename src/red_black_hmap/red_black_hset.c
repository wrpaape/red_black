#include "red_black_hmap/red_black_hset.h" /* HNode, comparator, NULL */


void
red_black_hset(struct RedBlackHNode *restrict node,
	       const struct RedBlackHKey *const restrict hkey)
{
	int compare;

	while (1) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			node->hkey.key = hkey->key;
			return;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}
