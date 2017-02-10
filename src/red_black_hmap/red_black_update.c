#include "red_black_tree/red_black_update.h"  /* types, JUMP API */
#include "red_black_tree/red_black_correct.h" /* post-insertion correction */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackUpdateHNode)(struct RedBlackHNode *restrict *const restrict tree,
		      struct RedBlackHNode *const restrict grandparent,
		      struct RedBlackHNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackHNodeFactory *const restrict factory,
		      RedBlackJumpBuffer jump_buffer,
		      const void *const key,
		      void **const restrict old_ptr);


/* update state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, old_ptr unset
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, successful key swap, old_ptr is set
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_update_ll(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_lr(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_rr(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_rl(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);


int
red_black_update(struct RedBlackHNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackHNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr)
{
	struct RedBlackHNode *restrict parent;
	const void *other_key;
	RedBlackUpdateHNode next_update;
	int compare;
	int status;

	struct RedBlackHNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		*tree = rbnf_new(factory,
				 jump_buffer,
				 key,
				 false); /* BLACK */
		return 1; /* tree updated */
	}

	other_key = grandparent->key;

	compare = comparator(key,
			     other_key);

	status = (compare != 0); /* 1, 0 */

	if (status) {
		if (compare < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = rbnf_new(factory,
							     jump_buffer,
							     key,
							     true); /* RED */

			} else {
				other_key = parent->key;

				compare = comparator(key,
						     other_key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_update = (compare < 0)
						    ? &rb_update_ll
						    : &rb_update_lr;

					(void) next_update(tree,
							   grandparent,
							   parent,
							   comparator,
							   factory,
							   jump_buffer,
							   key,
							   old_ptr);
					/* if return, tree must have updated */

				} else {
					parent->key = key;
					*old_ptr    = (void *) other_key;
				}
			}


		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = rbnf_new(factory,
							      jump_buffer,
							      key,
							      true); /* RED */

			} else {
				other_key = parent->key;

				compare = comparator(key,
						     other_key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_update = (compare < 0)
						    ? &rb_update_rl
						    : &rb_update_rr;

					(void) next_update(tree,
							   grandparent,
							   parent,
							   comparator,
							   factory,
							   jump_buffer,
							   key,
							   old_ptr);
					/* if return, tree must have updated */

				} else {
					parent->key = key;
					*old_ptr    = (void *) other_key;
				}
			}
		}

	} else {
		grandparent->key = key;
		*old_ptr         = (void *) other_key;
	}

	return status;
}



static bool
rb_update_ll(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateHNode next_update;
	struct RedBlackHNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		parent->left = rbnf_new(factory,
					jump_buffer,
					key,
					true); /* RED */

		/* need to correct */
		red_black_correct_ll_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		other_key = node->key;

		compare = comparator(key,
				     other_key);

		if (compare == 0) {
			node->key = key;
			*old_ptr  = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update = (compare < 0)
			    ? &rb_update_ll
			    : &rb_update_lr;

		status = next_update(&grandparent->left,
				     parent,
				     node,
				     comparator,
				     factory,
				     jump_buffer,
				     key,
				     old_ptr);

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
rb_update_lr(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateHNode next_update;
	struct RedBlackHNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		node = rbnf_new(factory,
				jump_buffer,
				key,
				true); /* RED */

		parent->right = node;

		/* need to correct */
		red_black_correct_lr_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		other_key = node->key;

		compare = comparator(key,
				     other_key);

		if (compare == 0) {
			node->key = key;
			*old_ptr  = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update = (compare < 0)
			    ? &rb_update_rl
			    : &rb_update_rr;

		status = next_update(&grandparent->left,
				     parent,
				     node,
				     comparator,
				     factory,
				     jump_buffer,
				     key,
				     old_ptr);

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
rb_update_rr(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateHNode next_update;
	struct RedBlackHNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		parent->right = rbnf_new(factory,
					 jump_buffer,
					 key,
					 true); /* RED */

		/* need to correct */
		red_black_correct_rr_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		other_key = node->key;

		compare = comparator(key,
				     other_key);

		if (compare == 0) {
			node->key = key;
			*old_ptr  = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update = (compare < 0)
			    ? &rb_update_rl
			    : &rb_update_rr;

		status = next_update(&grandparent->right,
				     parent,
				     node,
				     comparator,
				     factory,
				     jump_buffer,
				     key,
				     old_ptr);

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
rb_update_rl(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateHNode next_update;
	struct RedBlackHNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		node = rbnf_new(factory,
				jump_buffer,
				key,
				true); /* RED */

		parent->left = node;

		/* need to correct */
		red_black_correct_rl_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		other_key = node->key;

		compare = comparator(key,
				     other_key);

		if (compare == 0) {
			node->key = key;
			*old_ptr  = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update = (compare < 0)
			    ? &rb_update_ll
			    : &rb_update_lr;

		status = next_update(&grandparent->right,
				     parent,
				     node,
				     comparator,
				     factory,
				     jump_buffer,
				     key,
				     old_ptr);

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
