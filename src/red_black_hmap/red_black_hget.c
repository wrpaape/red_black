#include "red_black_hmap/red_black_hget.h" /* HNode */
#include "red_black_hmap/red_black_hkey.h" /* red_black_hkey_comparator */
#include <stddef.h>			   /* NULL */


void *
red_black_hget(const struct RedBlackHNode *restrict node,
	       const struct RedBlackHKey *const restrict hkey)
{
	int compare;

	while (1) {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0)
			return (void *) node->hkey.key;

		node = (compare < 0)
		     ? node->left
		     : node->right;
	}
}
