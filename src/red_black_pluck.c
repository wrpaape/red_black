#include "red_black_pluck.h"   /* Comparator, Node, NodeFactory, JumpBuffer */
#include "red_black_restore.h" /* restore API */


static inline void
rb_pluck_node(struct RedBlackNode *restrict *const restrict tree,
	      struct RedBlackNode *const restrict node,
	      struct RedBlackNodeFactory *const restrict factory,
	      RedBlackJumpBuffer *const restrict jump_buffer,
	      void **const restrict pluck_ptr)
{
	bool is_red;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	*pluck_ptr = (void *) node->key;
	is_red	   = node->is_red;
	lchild	   = node->left;
	rchild	   = node->right;

	/* free node */
	rbnf_free(factory,
		  node);

	if (is_red)
		red_black_restore_red(tree, /* always restorable if node is RED */
				      lchild,
				      rchild);
	else if (!red_black_restore_black(tree,
					  lchild,
					  rchild))
		return; /* need to restore */

	RED_BLACK_JUMP_2_TRUE(jump_buffer); /* all done */
}

static inline void
rb_pluck_root(struct RedBlackNode *restrict *const restrict tree,
	      struct RedBlackNode *const restrict root,
	      struct RedBlackNodeFactory *const restrict factory,
	      void **const restrict pluck_ptr)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	*pluck_ptr = (void *) root->key;
	lchild	   = root->left;
	rchild	   = root->right;

	/* free node */
	rbnf_free(factory,
		  root);

	(void) red_black_restore_black(tree,
				       lchild,
				       rchild);
}
/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackPluckNode)(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict parent,
		     const RedBlackComparator comparator,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer,
		     const void *const key,
		     void **const restrict pluck_ptr);

/* pluck state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_pluck_l(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer *const restrict jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr);
static void
rb_pluck_r(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer *const restrict jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr);

static void
rb_pluck_l(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer *const restrict jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;
	struct RedBlackNode *const restrict lnode	      = *subtree;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		rb_pluck_node(subtree,
			      lnode,
			      factory,
			      jump_buffer,
			      pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(subtree,
			   lnode,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_pluck_r(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer *const restrict jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;
	struct RedBlackNode *const restrict rnode	      = *subtree;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		rb_pluck_node(subtree,
			      rnode,
			      factory,
			      jump_buffer,
			      pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(subtree,
			   rnode,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



void
red_black_pluck(struct RedBlackNode *restrict *const restrict tree,
		const RedBlackComparator comparator,
		struct RedBlackNodeFactory *const restrict factory,
		RedBlackJumpBuffer *const restrict jump_buffer,
		const void *const key,
		void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *const restrict node = *tree;

	const int compare = comparator(key,
				       node->key);

	if (compare == 0) {
		rb_pluck_root(tree,
			      node,
			      factory,
			      pluck_ptr);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(tree,
			   node,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);
	}
}
