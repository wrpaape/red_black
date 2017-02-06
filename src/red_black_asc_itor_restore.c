#include "red_black_asc_itor_restore.h" /* Node, NodeFactory, Itor types */
#include "red_black_itor_restore.h"	/* function types, STACK_COUNT */


/* restore red-black properties ONLY */
static void
rbai_restore_valid_b_l(struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_valid_r_l(struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_valid_b_r(struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_valid_r_r(struct RedBlackItorNode *const restrict itor_node);

static const RedBlackItorRestoreValid rbai_restore_valid[] = {
	[RBI_PARENT_INFO_BLACK_LEFT]  = &rbai_restore_valid_b_l,
	[RBI_PARENT_INFO_RED_LEFT]    = &rbai_restore_valid_r_l,
	[RBI_PARENT_INFO_BLACK_RIGHT] = &rbai_restore_valid_b_r,
	[RBI_PARENT_INFO_RED_RIGHT]   = &rbai_restore_valid_r_r
};


/* restore cursor ONLY */
static void
rbai_restore_cursor_b_l(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_cursor_r_l(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_cursor_b_r(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_cursor_r_r(struct RedBlackItorCursor *const restrict cursor,
			struct RedBlackItorNode *const restrict itor_node);

static const RedBlackItorRestoreCursor rbai_restore_cursor[] = {
	[RBI_PARENT_INFO_BLACK_LEFT]  = &rbai_restore_cursor_b_l,
	[RBI_PARENT_INFO_RED_LEFT]    = &rbai_restore_cursor_r_l,
	[RBI_PARENT_INFO_BLACK_RIGHT] = &rbai_restore_cursor_b_r,
	[RBI_PARENT_INFO_RED_RIGHT]   = &rbai_restore_cursor_r_r
};


/* restore red-black properties AND cursor */
static void
rbai_restore_both_b_l(struct RedBlackItorCursor *const restrict cursor,
		      struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_both_r_l(struct RedBlackItorCursor *const restrict cursor,
		      struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_both_b_r(struct RedBlackItorCursor *const restrict cursor,
		      struct RedBlackItorNode *const restrict itor_node);
static void
rbai_restore_both_r_r(struct RedBlackItorCursor *const restrict cursor,
		      struct RedBlackItorNode *const restrict itor_node);

static const RedBlackItorRestoreBoth rbai_restore_both[] = {
	[RBI_PARENT_INFO_BLACK_LEFT]  = &rbai_restore_both_b_l,
	[RBI_PARENT_INFO_RED_LEFT]    = &rbai_restore_both_r_l,
	[RBI_PARENT_INFO_BLACK_RIGHT] = &rbai_restore_both_b_r,
	[RBI_PARENT_INFO_RED_RIGHT]   = &rbai_restore_both_r_r
};



static inline void
rbair_red_shallow(struct RedBlackItorCursor *const restrict cursor,
		  struct RedBlackItorNode *const restrict itor_node,
		  struct RedBlackNode *restrict *const restrict tree,
		  struct RedBlackNode *restrict *const restrict node_ptr,
		  struct RedBlackNode *const restrict lnode,
		  struct RedBlackNode *const restrict rnode)
{
	struct RedBlackNode *restrict lrchild;
	struct RedBlackNode *restrict rrchild;

	lrchild = lnode->right;

	if (lrchild == NULL) {
		rrchild = rnode->right;

		if (rrchild == NULL) {
			*tree = lnode;

			lnode->right = rnode;

			rnode->is_red = true;

		} else {
			*tree = rnode;

			rnode->is_red = true;
			rnode->left   = lnode;

			rrchild->is_red = false;
		}

	} else { /* lrchild MUST be RED leaf */
		*tree = lrchild;

		lrchild->left  = lnode;
		lrchild->right = rnode;

		lnode->right = NULL;
	}
}



static inline void
rbai_restore_red(struct RedBlackItorCursor *const restrict cursor,
		 struct RedBlackItorNode *const restrict itor_node,
		 struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *restrict *const restrict node_ptr,
		 struct RedBlackNode *const restrict lchild,
		 struct RedBlackNode *const restrict rchild)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	bool restored;

	struct RedBlackNode *restrict replacement_stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict replacement_cursor;

	if (lchild == NULL) {
		*tree = NULL; /* rchild must be NULL */

		/* need to restore cursor, no need to check if root (RED) */
		rbai_restore_both[itor_node->parent_info](cursor,
							  itor_node - 1);
		return;
	}

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->replacement_parent->replacement
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;
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
