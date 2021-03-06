#include "red_black_tree/red_black_add.h"     /* types, JUMP API */
#include "red_black_tree/red_black_correct.h" /* post-insertion correction */
#include <stddef.h>			      /* NULL */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackAddNode)(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *const restrict grandparent,
		   struct RedBlackNode *const restrict parent,
		   const RedBlackComparator comparator,
		   RedBlackJumpBuffer jump_buffer,
		   struct RedBlackNode *const restrict node);


/* put state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, tree updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_add_ll(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node);
static bool
rb_add_lr(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node);
static bool
rb_add_rr(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node);
static bool
rb_add_rl(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node);


void
red_black_add(struct RedBlackNode *restrict *const restrict tree,
	      const RedBlackComparator comparator,
	      RedBlackJumpBuffer jump_buffer,
	      struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict parent;
	RedBlackAddNode next_add;

	struct RedBlackNode *const restrict grandparent = *tree;

	const bool not_empty = (grandparent != NULL);

	node->is_red = not_empty; /* BLACK if root, RED otherwise */
	node->left   = NULL;      /* make 'node' a leaf */
	node->right  = NULL;

	if (not_empty) {
		const void *const key = node->key;

		if (comparator(key,
			       grandparent->key) < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = node; /* put RED leaf */

			} else {
				next_add = (comparator(key,
						       parent->key) < 0)
					 ? &rb_add_ll
					 : &rb_add_lr;

				(void) next_add(tree,
						grandparent,
						parent,
						comparator,
						jump_buffer,
						node);
			}

		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = node; /* put RED leaf */

			} else {
				next_add = (comparator(key,
						       parent->key) < 0)
					 ? &rb_add_rl
					 : &rb_add_rr;

				(void) next_add(tree,
						grandparent,
						parent,
						comparator,
						jump_buffer,
						node);
			}
		}

	} else {
		*tree = node; /* make node new root */
	}
}



static bool
rb_add_ll(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAddNode next_add;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* put RED leaf */

		/* need to correct */
		red_black_correct_ll_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		next_add = (comparator(node->key,
				       next->key) < 0)
			 ? &rb_add_ll
			 : &rb_add_lr;

		status = next_add(&grandparent->left,
				  parent,
				  next,
				  comparator,
				  jump_buffer,
				  node);

		if (status) /* need to correct */
			red_black_correct_ll_mid(tree,
						 grandparent,
						 parent,
						 jump_buffer);
		else /* need to recolor */
			parent->is_red = true;
	}

	/* if had to correct and didn't jump, need to recolor in previous frame,
	 * otherwise if had to recolor, need to correct in previous frame */
	return !status;
}


static bool
rb_add_lr(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAddNode next_add;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* put RED leaf */

		/* need to correct */
		red_black_correct_lr_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		next_add = (comparator(node->key,
				       next->key) < 0)
			 ? &rb_add_rl
			 : &rb_add_rr;

		status = next_add(&grandparent->left,
				  parent,
				  next,
				  comparator,
				  jump_buffer,
				  node);

		if (status) /* need to correct */
			red_black_correct_lr_mid(tree,
						 grandparent,
						 parent,
						 parent->right,
						 jump_buffer);
		else /* need to recolor */
			parent->is_red = true;
	}

	/* if had to correct and didn't jump, need to recolor in previous frame,
	 * otherwise if had to recolor, need to correct in previous frame */
	return !status;
}


static bool
rb_add_rr(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAddNode next_add;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* put RED leaf */

		/* need to correct */
		red_black_correct_rr_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		next_add = (comparator(node->key,
				       next->key) < 0)
			 ? &rb_add_rl
			 : &rb_add_rr;

		status = next_add(&grandparent->right,
				  parent,
				  next,
				  comparator,
				  jump_buffer,
				  node);

		if (status) /* need to correct */
			red_black_correct_rr_mid(tree,
						 grandparent,
						 parent,
						 jump_buffer);
		else /* need to recolor */
			parent->is_red = true;
	}

	/* if had to correct and didn't jump, need to recolor in previous frame,
	 * otherwise if had to recolor, need to correct in previous frame */
	return !status;
}


static bool
rb_add_rl(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict grandparent,
	  struct RedBlackNode *const restrict parent,
	  const RedBlackComparator comparator,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAddNode next_add;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* put RED leaf */

		/* need to correct */
		red_black_correct_rl_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		next_add = (comparator(node->key,
					  next->key) < 0)
			 ? &rb_add_ll
			 : &rb_add_lr;

		status = next_add(&grandparent->right,
				  parent,
				  next,
				  comparator,
				  jump_buffer,
				  node);

		if (status) /* need to correct */
			red_black_correct_rl_mid(tree,
						 grandparent,
						 parent,
						 parent->left,
						 jump_buffer);
		else /* need to recolor */
			parent->is_red = true;
	}

	/* if had to correct and didn't jump, need to recolor in previous frame,
	 * otherwise if had to recolor, need to correct in previous frame */
	return !status;
}
