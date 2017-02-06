#include "red_black_asc_itor_restore.h" /* Node, NodeFactory, Itor types */
#include "red_black_stack_count.h"      /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		        /* NULL */

static inline void
rbai_restore_red(struct RedBlackItorCursor *const restrict cursor,
		 struct RedBlackItorNode *const restrict itor_node,
		 struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *restrict *const restrict node_ptr,
		 struct RedBlackNode *const restrict lchild,
		 struct RedBlackNode *const restrict rchild)
{
}


void
red_black_asc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackItorNode *restrict itor_node;
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict *restrict node_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	bool is_red;

	itor_node = cursor->path;

	tree     = itor_node->tree;
	node_ptr = &itor_node->node;

	node = *node_ptr;

	is_red = node->is_red;
	lchild = node->left;
	rchild = node->right;

	/* free node */
	rbnf_free(factory,
		  node);

	if (is_red)
		rbai_restore_red(cursor,
				 itor_node,
				 tree,
				 node_ptr,
				 lchild,
				 rchild);
	else
		rbai_restore_black(cursor,
				   itor_node,
				   tree,
				   node_ptr,
				   lchild,
				   rchild);
}
