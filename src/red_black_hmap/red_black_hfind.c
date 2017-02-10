#include "red_black_hmap/red_black_hfind.h" /* HNode */
#include "red_black_hmap/red_black_hkey.h"  /* red_black_hkey_comparator */
#include <stddef.h>			    /* NULL */


bool
red_black_hfind(const struct RedBlackHNode *restrict node,
		const struct RedBlackHKey *const restrict hkey)
{
	int compare;

	while (node != NULL) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare < 0)
			node = node->left;
		else if (compare > 0)
			node = node->right;
		else
			return true;
	}

	return false;
}
