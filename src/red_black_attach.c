#include "red_black_attach.h"  /* Node, Comparator, Allocator, JumpBuffer */
#include "red_black_correct.h" /* post-insertion correction functions */
#include <stddef.h>	       /* NULL */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackAttachNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict grandparent,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      struct RedBlackNode *const restrict node);


/* attach state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, tree updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_attach_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node);
static bool
rb_attach_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node);
static bool
rb_attach_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node);
static bool
rb_attach_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node);


void
red_black_attach(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict parent;
	RedBlackAttachNode next_attach;

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
				grandparent->left = node; /* attach RED leaf */

			} else {
				next_attach = (comparator(key,
							  parent->key) < 0)
					    ? &rb_attach_ll
					    : &rb_attach_lr;

				(void) next_attach(tree,
						   grandparent,
						   parent,
						   comparator,
						   jump_buffer,
						   node);
			}

		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = node; /* attach RED leaf */

			} else {
				next_attach = (comparator(key,
							  parent->key) < 0)
					    ? &rb_attach_rl
					    : &rb_attach_rr;

				(void) next_attach(tree,
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
rb_attach_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAttachNode next_attach;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* attach RED leaf */

		/* need to correct */
		red_black_correct_ll_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		next_attach = (comparator(node->key,
					  next->key) < 0)
			    ? &rb_attach_ll
			    : &rb_attach_lr;

		status = next_attach(&grandparent->left,
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
rb_attach_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAttachNode next_attach;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* attach RED leaf */

		/* need to correct */
		red_black_correct_lr_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		next_attach = (comparator(node->key,
					  next->key) < 0)
			    ? &rb_attach_rl
			    : &rb_attach_rr;

		status = next_attach(&grandparent->left,
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
rb_attach_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAttachNode next_attach;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* attach RED leaf */

		/* need to correct */
		red_black_correct_rr_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		next_attach = (comparator(node->key,
					  next->key) < 0)
			    ? &rb_attach_rl
			    : &rb_attach_rr;

		status = next_attach(&grandparent->right,
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
rb_attach_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     struct RedBlackNode *const restrict node)
{
	bool status;
	struct RedBlackNode *restrict next;
	RedBlackAttachNode next_attach;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* attach RED leaf */

		/* need to correct */
		red_black_correct_rl_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		next_attach = (comparator(node->key,
					  next->key) < 0)
			    ? &rb_attach_ll
			    : &rb_attach_lr;

		status = next_attach(&grandparent->right,
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
