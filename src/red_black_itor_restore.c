#include "red_black_itor_restore.h" /* Itor types, restore deps */

static void
rbi_restore_valid_b_l(struct RedBlackItorNode *const restrict itor_node)
{
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict *restrict node_ptr;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict rnode;
	struct RedBlackNode *restrict rrchild;
	struct RedBlackNode *restrict rlchild;
	struct RedBlackNode *restrict rllgrandchild;

	tree     = itor_node->tree;
	node_ptr = &itor_node->node;
	parent   = *node_ptr;

	rnode   = parent->right;
	rlchild = rnode->left;

	if (rnode->is_red) {
		rllgrandchild = rlchild->left;

		if (rllgrandchild->is_red) {
			rllgrandchild->is_red = false;
			parent->right         = rllgrandchild->left;
			rllgrandchild->left   = parent;

			rlchild->left	     = rllgrandchild->right;
			rllgrandchild->right = rnode;

			*tree     = rllgrandchild;
			*node_ptr = rllgrandchild;

		} else {
			rlchild->left  = parent;
			parent->is_red = true;
			parent->right  = rllgrandchild;

			*tree     = rnode;
			*node_ptr = rnode;

			rnode->is_red = false;
		}

	} else if (rlchild->is_red) {
		rlchild->is_red = false;
		parent->right   = rlchild->left;
		rlchild->left   = parent;

		rnode->left    = rlchild->right;
		rlchild->right = rnode;

		*tree     = rlchild;
		*node_ptr = rlchild;

	} else {
		rrchild = rnode->right;

		if (rrchild->is_red) {
			rrchild->is_red = false;

			*tree	  = rnode;
			*node_ptr = rnode;

			rnode->left   = parent;
			parent->right = rlchild;

		} else {
			rnode->is_red = true;
			/* balanced but deficient 1 black height -> recurse */
			rbi_restore_valid_b(itor_node);
		}
	}
}


static void
rbi_restore_valid_r_l(struct RedBlackItorNode *const restrict itor_node)
{
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict *restrict node_ptr;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict rnode;
	struct RedBlackNode *restrict rrchild;
	struct RedBlackNode *restrict rlchild;

	tree     = itor_node->tree;
	node_ptr = &itor_node->node;
	parent   = *node_ptr;

	rnode   = parent->right;
	rlchild = rnode->left;

	if (rlchild->is_red) {
		rrchild = rnode->right;

		if (rrchild->is_red) {
			rrchild->is_red = false;

			*tree     = rnode;
			*node_ptr = rnode;

			rnode->is_red = true;
			rnode->left   = parent;

			parent->is_red = false;
			parent->right  = rlchild;

		} else {
			*tree	  = rlchild;
			*node_ptr = rlchild;

			parent->is_red = false;
			parent->right  = rlchild->left;
			rlchild->left  = parent;

			rnode->left    = rlchild->right;
			rlchild->right = rnode;
		}

	} else {
		*tree	  = rnode;
		*node_ptr = rnode;

		rnode->left   = parent;
		parent->right = rlchild;
	}
}


static void
rbi_restore_valid_b_r(struct RedBlackItorNode *const restrict itor_node)
{
}


static void
rbi_restore_valid_r_r(struct RedBlackItorNode *const restrict itor_node)
{
}


/* restore red-black properties ONLY */
typedef void
(*RedBlackItorRestoreValid)(struct RedBlackItorNode *const restrict itor_node);

void
rbi_restore_valid_b(struct RedBlackItorNode *const restrict itor_node)
{
	/* use table for 4 options (parent can be red/black left/right) */
	static const RedBlackItorRestoreValid rbi_restore_valid[] = {
		[RBIN_PARENT_INFO_BLACK_LEFT]  = &rbi_restore_valid_b_l,
		[RBIN_PARENT_INFO_RED_LEFT]    = &rbi_restore_valid_r_l,
		[RBIN_PARENT_INFO_BLACK_RIGHT] = &rbi_restore_valid_b_r,
		[RBIN_PARENT_INFO_RED_RIGHT]   = &rbi_restore_valid_r_r
	};

	const int parent_info = itor_node->parent_info;

	if (parent_info >= 0)
		rbi_restore_valid[parent_info](itor_node - 1);
}

void
rbi_restore_valid_r(struct RedBlackItorNode *const restrict itor_node)
{
	struct RedBlackItor *const restrict parent_itor_node = itor_node - 1;

	/* use comparison for 2 options (parent can be black left/right) */
	if (itor_node->parent_info == RBIN_PARENT_INFO_BLACK_LEFT)
		rbi_restore_valid_b_l(parent_itor_node);
	else
		rbi_restore_valid_b_r(parent_itor_node);
}
