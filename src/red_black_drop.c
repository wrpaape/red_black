#include "red_black_drop.h"    /* Comparator, Node, NodeFactory, JumpBuffer */
#include "red_black_restore.h" /* restore API */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackDropNode)(struct RedBlackNode *restrict *const restrict tree,
		    struct RedBlackNode *const restrict parent,
		    const RedBlackComparator comparator,
		    struct RedBlackNodeFactory *const restrict factory,
		    RedBlackJumpBuffer *const restrict jump_buffer,
		    const void *const key);

/* drop state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_drop_l(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  struct RedBlackNodeFactory *const restrict factory,
	  RedBlackJumpBuffer *const restrict jump_buffer,
	  const void *const key);
static void
rb_drop_r(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  struct RedBlackNodeFactory *const restrict factory,
	  RedBlackJumpBuffer *const restrict jump_buffer,
	  const void *const key);

static void
rb_drop_l(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  struct RedBlackNodeFactory *const restrict factory,
	  RedBlackJumpBuffer *const restrict jump_buffer,
	  const void *const key)
{
	RedBlackDropNode next_drop;

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;
	struct RedBlackNode *const restrict lnode	      = *subtree;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       lnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_drop = (compare < 0)
			  ? &rb_drop_l
			  : &rb_drop_r;

		next_drop(subtree,
			  lnode,
			  comparator,
			  factory,
			  jump_buffer,
			  key);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_drop_r(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  struct RedBlackNodeFactory *const restrict factory,
	  RedBlackJumpBuffer *const restrict jump_buffer,
	  const void *const key)
{
	RedBlackDropNode next_drop;

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;
	struct RedBlackNode *const restrict rnode	      = *subtree;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       rnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_drop = (compare < 0)
			  ? &rb_drop_l
			  : &rb_drop_r;

		next_drop(subtree,
			  rnode,
			  comparator,
			  factory,
			  jump_buffer,
			  key);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



void
red_black_drop(struct RedBlackNode *restrict *const restrict tree,
	       const RedBlackComparator comparator,
	       struct RedBlackNodeFactory *const restrict factory,
	       RedBlackJumpBuffer *const restrict jump_buffer,
	       const void *const key)
{
	RedBlackDropNode next_drop;
	struct RedBlackNode *const restrict root = *tree;

	const int compare = comparator(key,
				       root->key);

	if (compare == 0) {
		red_black_restore_root(tree,
				       root,
				       factory);

	} else {
		next_drop = (compare < 0)
			  ? &rb_drop_l
			  : &rb_drop_r;

		next_drop(tree,
			  root,
			  comparator,
			  factory,
			  jump_buffer,
			  key);
	}
}
