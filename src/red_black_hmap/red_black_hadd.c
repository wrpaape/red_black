#include "red_black_hmap/red_black_hadd.h"     /* types, JUMP API */
#include "red_black_hmap/red_black_hcorrect.h" /* post-insertion correction */
#include "red_black_hmap/red_black_hkey.h"     /* red_black_hkey_comparator */
#include <stddef.h>			       /* NULL */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackHAddNode)(struct RedBlackHNode *restrict *const restrict tree,
		    struct RedBlackHNode *const restrict grandparent,
		    struct RedBlackHNode *const restrict parent,
		    RedBlackJumpBuffer jump_buffer,
		    struct RedBlackHNode *const restrict node);


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
rb_hadd_ll(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict grandparent,
	   struct RedBlackHNode *const restrict parent,
	   RedBlackJumpBuffer jump_buffer,
	   struct RedBlackHNode *const restrict node);
static bool
rb_hadd_lr(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict grandparent,
	   struct RedBlackHNode *const restrict parent,
	   RedBlackJumpBuffer jump_buffer,
	   struct RedBlackHNode *const restrict node);
static bool
rb_hadd_rr(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict grandparent,
	   struct RedBlackHNode *const restrict parent,
	   RedBlackJumpBuffer jump_buffer,
	   struct RedBlackHNode *const restrict node);
static bool
rb_hadd_rl(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict grandparent,
	   struct RedBlackHNode *const restrict parent,
	   RedBlackJumpBuffer jump_buffer,
	   struct RedBlackHNode *const restrict node);


void
red_black_hadd(struct RedBlackHNode *restrict *const restrict tree,
	       RedBlackJumpBuffer jump_buffer,
	       struct RedBlackHNode *const restrict node)
{
	struct RedBlackHNode *restrict parent;
	RedBlackHAddNode next_hadd;

	struct RedBlackHNode *const restrict grandparent = *tree;

	const bool not_empty = (grandparent != NULL);

	node->is_red = not_empty; /* BLACK if root, RED otherwise */
	node->left   = NULL;      /* make 'node' a leaf */
	node->right  = NULL;

	if (not_empty) {
		const struct RedBlackHKey *const restrict hkey = &node->hkey;

		if (red_black_hkey_comparator(hkey,
					      &grandparent->hkey) < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = node; /* put RED leaf */

			} else {
				next_hadd
				= (red_black_hkey_comparator(hkey,
							     &parent->hkey) < 0)
				? &rb_hadd_ll
				: &rb_hadd_lr;

				(void) next_hadd(tree,
						 grandparent,
						 parent,
						 jump_buffer,
						 node);
			}

		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = node; /* put RED leaf */

			} else {
				next_hadd
				= (red_black_hkey_comparator(hkey,
							     &parent->hkey) < 0)
				? &rb_hadd_rl
				: &rb_hadd_rr;

				(void) next_hadd(tree,
						 grandparent,
						 parent,
						 jump_buffer,
						 node);
			}
		}

	} else {
		*tree = node; /* make node new root */
	}
}



static bool
rb_hadd_ll(struct RedBlackHNode *restrict *const restrict tree,
	  struct RedBlackHNode *const restrict grandparent,
	  struct RedBlackHNode *const restrict parent,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackHNode *const restrict node)
{
	bool status;
	struct RedBlackHNode *restrict next;
	RedBlackHAddNode next_hadd;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* put RED leaf */

		/* need to correct */
		red_black_hcorrect_ll_bot(tree,
					  grandparent,
					  parent,
					  jump_buffer);

	} else {
		next_hadd = (red_black_hkey_comparator(&node->hkey,
						       &next->hkey) < 0)
			  ? &rb_hadd_ll
			  : &rb_hadd_lr;

		status = next_hadd(&grandparent->left,
				   parent,
				   next,
				   jump_buffer,
				   node);

		if (status) /* need to correct */
			red_black_hcorrect_ll_mid(tree,
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
rb_hadd_lr(struct RedBlackHNode *restrict *const restrict tree,
	  struct RedBlackHNode *const restrict grandparent,
	  struct RedBlackHNode *const restrict parent,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackHNode *const restrict node)
{
	bool status;
	struct RedBlackHNode *restrict next;
	RedBlackHAddNode next_hadd;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* put RED leaf */

		/* need to correct */
		red_black_hcorrect_lr_bot(tree,
					  grandparent,
					  parent,
					  node,
					  jump_buffer);

	} else {
		next_hadd = (red_black_hkey_comparator(&node->hkey,
						       &next->hkey) < 0)
			  ? &rb_hadd_rl
			  : &rb_hadd_rr;

		status = next_hadd(&grandparent->left,
				   parent,
				   next,
				   jump_buffer,
				   node);

		if (status) /* need to correct */
			red_black_hcorrect_lr_mid(tree,
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
rb_hadd_rr(struct RedBlackHNode *restrict *const restrict tree,
	  struct RedBlackHNode *const restrict grandparent,
	  struct RedBlackHNode *const restrict parent,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackHNode *const restrict node)
{
	bool status;
	struct RedBlackHNode *restrict next;
	RedBlackHAddNode next_hadd;

	next = parent->right;

	status = (next == NULL);

	if (status) {
		parent->right = node; /* put RED leaf */

		/* need to correct */
		red_black_hcorrect_rr_bot(tree,
					  grandparent,
					  parent,
					  jump_buffer);

	} else {
		next_hadd = (red_black_hkey_comparator(&node->hkey,
						       &next->hkey) < 0)
			  ? &rb_hadd_rl
			  : &rb_hadd_rr;

		status = next_hadd(&grandparent->right,
				   parent,
				   next,
				   jump_buffer,
				   node);

		if (status) /* need to correct */
			red_black_hcorrect_rr_mid(tree,
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
rb_hadd_rl(struct RedBlackHNode *restrict *const restrict tree,
	  struct RedBlackHNode *const restrict grandparent,
	  struct RedBlackHNode *const restrict parent,
	  RedBlackJumpBuffer jump_buffer,
	  struct RedBlackHNode *const restrict node)
{
	bool status;
	struct RedBlackHNode *restrict next;
	RedBlackHAddNode next_hadd;

	next = parent->left;

	status = (next == NULL);

	if (status) {
		parent->left = node; /* put RED leaf */

		/* need to correct */
		red_black_hcorrect_rl_bot(tree,
					  grandparent,
					  parent,
					  node,
					  jump_buffer);

	} else {
		next_hadd = (red_black_hkey_comparator(&node->hkey,
						       &next->hkey) < 0)
			  ? &rb_hadd_ll
			  : &rb_hadd_lr;

		status = next_hadd(&grandparent->right,
				   parent,
				   next,
				   jump_buffer,
				   node);

		if (status) /* need to correct */
			red_black_hcorrect_rl_mid(tree,
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
