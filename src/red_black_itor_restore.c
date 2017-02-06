#include "red_black_itor_restore.h" /* Node, NodeFactory, Itor types */
#include "red_black_stack_count.h"  /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		    /* NULL */

/* typedef void */
/* (*RedBlackItorRestoreNode)(struct RedBlackItorCursor *const restrict cursor, */
/* 			   struct RedBlackItorNode *const restrict itor_root, */
/* 			   struct RedBlackItorNode *const restrict itor_node, */
/* 			   struct RedBlackNode *const restrict lchild, */
/* 			   struct RedBlackNode *restrict rchild); */

static inline void
rbai_restore_both_bot_l_l(struct RedBlackItorCursor *const restrict cursor,
			  struct RedBlackItorNode *const restrict itor_root,
			  struct RedBlackItorNode *const restrict itor_node)

static inline void
rbai_restore_both_bot_r(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_root,
			struct RedBlackItorNode *const restrict itor_node)
{
}

static inline void
rbai_restore_both_bot_b(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_root,
			struct RedBlackItorNode *const restrict itor_node)
{
}

static inline void
rbai_restore_both_bot(struct RedBlackItorCursor *const restrict cursor,
		      struct RedBlackItorNode *const restrict itor_root,
		      struct RedBlackItorNode *const restrict itor_node)
{
	struct RedBlackItorNode *restrict itor_node;
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict *restrict node_ptr;
	struct RedBlackNode *restrict parent;

	tree     = itor_node->tree;
	node_ptr = &itor_node->node;

	parent = *node_ptr;

	if (itor_node->subtree_is_left) {
		if (parent->is_red)
			rbai_restore_both_bot_l_l(cursor,
						  itor_root,
						  itor_node,
						  tree,
						  node_ptr,
						  parent);
		else
			rbai_restore_both_bot_l_b(cursor,
						  itor_root,
						  itor_node,
						  tree,
						  node_ptr,
						  parent);

	} else {
	}
}

static inline void
rbai_restore_red(struct RedBlackItorCursor *const restrict cursor,
		 struct RedBlackItorNode *const restrict itor_root,
		 struct RedBlackItorNode *const restrict itor_node,
		 struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *restrict *const restrict node_ptr,
		 struct RedBlackNode *const restrict lchild,
		 struct RedBlackNode *restrict rchild)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	bool restored;

	struct RedBlackNode *restrict replacement_stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict replacement_cursor;

	if (lchild == NULL) {
		*tree = NULL; /* rchild must be NULL */

		/* need to restore cursor AND balance */
		rbai_restore_both(cursor,
				  itor_root,
				  itor_node - 1);
		return;
	}

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->replacement_parent->replacement
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		rb_restore_red_shallow(tree,
				       lchild,
				       rchild);
		return;
	}

	replacement_cursor  = &replacement_stack[0];
	*replacement_cursor = NULL;

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		++replacement_cursor;
		*replacement_cursor = replacement_parent;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	restored = rb_restore_red_rtree(&rchild,
					replacement_cursor,
					replacement_parent,
					replacement,
					replacement_child);

	replacement->right = rchild; /* set right subtree of replacement */

	if (restored) {
		/* black height restored through recoloring and/or rotation of
		 * ancestors or right child of replacement
		 *
		 * replacement is RED */
		*tree = replacement;
		replacement->left = lchild;

	} else {
		rb_restore_red_ltree(tree,
				     lchild,
				     replacement);
	}
}


static inline void
rbai_restore_black(struct RedBlackItorCursor *const restrict cursor,
		   struct RedBlackItorNode *const restrict itor_root,
		   struct RedBlackItorNode *const restrict itor_node,
		   struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *restrict *const restrict node_ptr,
		   struct RedBlackNode *const restrict lchild,
		   struct RedBlackNode *restrict rchild)
{
}


void
red_black_asc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_root,
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
				 itor_root,
				 itor_node,
				 tree,
				 node_ptr,
				 lchild,
				 rchild);
	else
		rbai_restore_black(cursor,
				   itor_root,
				   itor_node,
				   tree,
				   node_ptr,
				   lchild,
				   rchild);
}

void
red_black_desc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			    struct RedBlackItorNode *const restrict itor_root
			    struct RedBlackNodeFactory *const restrict factory)
{
}
