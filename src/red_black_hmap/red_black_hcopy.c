#include "red_black_tree/red_black_copy.h"  /* RedBlackNode */
#include <stddef.h>			    /* NULL */


struct RedBlackNode *
red_black_copy_nodes(struct RedBlackNode *restrict *const restrict dst_tree,
		     const struct RedBlackNode *const restrict src_node,
		     struct RedBlackNode *restrict buffer)
{
	struct RedBlackNode *restrict dst_node;

	if (src_node == NULL) {
		*dst_tree = NULL;

	} else {
		dst_node = buffer++;

		*dst_tree = dst_node;

		*dst_node = *src_node;

		buffer = red_black_copy_nodes(&dst_node->left,
					      src_node->left,
					      buffer);

		buffer = red_black_copy_nodes(&dst_node->right,
					      src_node->right,
					      buffer);
	}

	return buffer;
}


/* struct RedBlackNode * */
/* red_black_copy_hnodes(struct RedBlackNode *restrict *const restrict dst_tree, */
/* 		      const struct RedBlackNode *const restrict src_node, */
/* 		      struct RedBlackNode *restrict buffer) */
/* { */
/* 	struct RedBlackHNode *restrict dst_node; */

/* 	if (src_node == NULL) { */
/* 		*dst_tree = NULL; */

/* 	} else { */
/* 		dst_node = (struct RedBlackHNode *restrict) buffer; */

/* 		buffer = (struct RedBlackNode *restrict) (dst_node + 1); */

/* 		*dst_tree = (struct RedBlackNode *restrict) dst_node; */

/* 		*dst_node = *((const struct RedBlackHNode *restrict) src_node); */

/* 		buffer = red_black_copy_hnodes(&dst_node->node.left, */
/* 					       src_node->left, */
/* 					       buffer); */

/* 		buffer = red_black_copy_hnodes(&dst_node->node.right, */
/* 					       src_node->right, */
/* 					       buffer); */
/* 	} */

/* 	return buffer; */
/* } */
