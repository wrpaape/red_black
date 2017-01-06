#include "red_black_update.h"  /* Node, Comparator, Allocator, JumpBuffer */
#include "red_black_correct.h" /* post-insertion correction functions */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackUpdateNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict grandparent,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackAllocator *const restrict allocator,
		      RedBlackJumpBuffer *const restrict jump_buffer,
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
rb_update_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);
static bool
rb_update_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr);


int
red_black_update(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr)
{
	struct RedBlackNode *restrict parent;
	const void *other_key;
	RedBlackUpdateNode next_update;
	int compare;
	int status;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		*tree = red_black_allocator_new(allocator,
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
				grandparent->left
				= red_black_allocator_new(allocator,
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
							   comparator,
							   allocator,
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
				grandparent->right
				= red_black_allocator_new(allocator,
							  jump_buffer,
							  key,
							  true); /* RED */

			} else {
				compare = comparator(key,
						     parent->key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_update = (compare < 0)
						    ? &rb_update_rl
						    : &rb_update_rr;

					(void) next_update(tree,
							   grandparent,
							   comparator,
							   allocator,
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
rb_update_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateNode next_update;
	struct RedBlackNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		parent->left = red_black_allocator_new(allocator,
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
				     allocator,
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
rb_update_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateNode next_update;
	struct RedBlackNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		node = red_black_allocator_new(allocator,
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
				     allocator,
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
rb_update_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateNode next_update;
	struct RedBlackNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		parent->right = red_black_allocator_new(allocator,
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
				     allocator,
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
rb_update_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateNode next_update;
	struct RedBlackNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		node = red_black_allocator_new(allocator,
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
				     allocator,
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
