#include "red_black_insert.h"  /* Node, Comparator, Allocator, JumpBuffer */
#include "red_black_correct.h" /* post-insertion correction functions */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackInsertNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict grandparent,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackAllocator *const restrict allocator,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key);


/* insert state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no insertion made, tree NOT updated
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key);
static bool
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key);
static bool
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key);
static bool
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key);


int
red_black_insert(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	struct RedBlackNode *restrict parent;
	RedBlackInsertNode next_insert;
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

	compare = comparator(key,
			     grandparent->key);

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
				compare = comparator(key,
						     parent->key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_insert = (compare < 0)
						    ? &rb_insert_ll
						    : &rb_insert_lr;

					(void) next_insert(tree,
                                                           grandparent,
                                                           parent,
                                                           comparator,
                                                           allocator,
                                                           jump_buffer,
                                                           key);
					/* if return, tree must have updated */
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
					next_insert = (compare < 0)
						    ? &rb_insert_rl
						    : &rb_insert_rr;

					(void) next_insert(tree,
                                                           grandparent,
                                                           parent,
                                                           comparator,
                                                           allocator,
                                                           jump_buffer,
                                                           key);
					/* if return, tree must have updated */
				}
			}
		}
	}

	return status;
}



static bool
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key)
{
	bool status;
	int compare;
	struct RedBlackNode *restrict node;
	RedBlackInsertNode next_insert;

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
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_insert = (compare < 0)
			    ? &rb_insert_ll
			    : &rb_insert_lr;

		status = next_insert(&grandparent->left,
				     parent,
				     node,
				     comparator,
				     allocator,
				     jump_buffer,
				     key);

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
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key)
{
	bool status;
	int compare;
	struct RedBlackNode *restrict node;
	RedBlackInsertNode next_insert;

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
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_insert = (compare < 0)
			    ? &rb_insert_rl
			    : &rb_insert_rr;

		status = next_insert(&grandparent->left,
				     parent,
				     node,
				     comparator,
				     allocator,
				     jump_buffer,
				     key);

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
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key)
{
	bool status;
	int compare;
	struct RedBlackNode *restrict node;
	RedBlackInsertNode next_insert;

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
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_insert = (compare < 0)
			    ? &rb_insert_rl
			    : &rb_insert_rr;

		status = next_insert(&grandparent->right,
				     parent,
				     node,
				     comparator,
				     allocator,
				     jump_buffer,
				     key);

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
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackAllocator *const restrict allocator,
	     RedBlackJumpBuffer *const restrict jump_buffer,
	     const void *const key)
{
	bool status;
	int compare;
	struct RedBlackNode *restrict node;
	RedBlackInsertNode next_insert;

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
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_insert = (compare < 0)
			    ? &rb_insert_ll
			    : &rb_insert_lr;

		status = next_insert(&grandparent->right,
				     parent,
				     node,
				     comparator,
				     allocator,
				     jump_buffer,
				     key);

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
