#include "red_black_tree/red_black_copy.h" /* RedBlackNode */
#include <stddef.h>			   /* NULL */


struct RedBlackNode *
red_black_copy(struct RedBlackNode *restrict *const restrict dst_tree,
	       const struct RedBlackNode *const restrict src_node,
	       struct RedBlackNode *restrict buffer)
{
	struct RedBlackNode *restrict dst_node;

	if (src_node == NULL) {
		*dst_tree = NULL;

	} else {
		dst_node = buffer++;

		*dst_tree = dst_node;

		dst_node->key    = src_node->key;
		dst_node->is_red = src_node->is_red;

		buffer = red_black_copy(&dst_node->left,
					src_node->left,
					buffer);

		buffer = red_black_copy(&dst_node->right,
					src_node->right,
					buffer);
	}

	return buffer;
}
