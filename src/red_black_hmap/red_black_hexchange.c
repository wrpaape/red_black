#include "red_black_hmap/red_black_hexchange.h" /* HNode */
#include "red_black_hmap/red_black_hkey.h"      /* red_black_hkey_comparator */
#include <stddef.h>			        /* NULL */

bool
red_black_hexchange(struct RedBlackHNode *restrict node,
		    const struct RedBlackHKey *const restrict hkey,
		    void **const restrict old_ptr)
{
	int compare;

	while (node != NULL) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			*old_ptr       = (void *) node->hkey.key;
			node->hkey.key = hkey->key;
			return true;
		}

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}

	return false;
}
