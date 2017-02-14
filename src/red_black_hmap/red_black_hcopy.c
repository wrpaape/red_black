#include "red_black_hmap/red_black_hcopy.h" /* RedBlackHNode */
#include <stddef.h>			    /* NULL */


struct RedBlackHNode *
red_black_hcopy(struct RedBlackHNode *restrict *const restrict dst_tree,
		const struct RedBlackHNode *const restrict src_node,
		struct RedBlackHNode *restrict buffer)
{
	struct RedBlackHNode *restrict dst_node;

	if (src_node == NULL) {
		*dst_tree = NULL;

	} else {
		dst_node = buffer++;

		dst_node->hkey   = src_node->hkey;
		dst_node->is_red = src_node->is_red;

		*dst_node = *src_node;

		buffer = red_black_hcopy(&dst_node->left,
					 src_node->left,
					 buffer);

		buffer = red_black_hcopy(&dst_node->right,
					 src_node->right,
					 buffer);
	}

	return buffer;
}
