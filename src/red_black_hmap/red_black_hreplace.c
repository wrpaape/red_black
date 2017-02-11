#include "red_black_hmap/red_black_hreplace.h" /* HNode, hkey_comparator, NULL */

bool
red_black_hreplace(struct RedBlackHNode *restrict node,
		   const struct RedBlackHKey *const restrict hkey)
{
	int compare;

	while (node != NULL) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			node->hkey.key = hkey->key;
			return true;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}

	return false;
}
