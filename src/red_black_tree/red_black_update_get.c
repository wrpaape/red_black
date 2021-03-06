#include "red_black_tree/red_black_update_get.h"  /* types, JUMP API */
#include "red_black_tree/red_black_correct.h"	  /* insertion correction */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackUpdateGetNode)(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 const RedBlackComparator comparator,
			 struct RedBlackNodeFactory *const restrict factory,
			 RedBlackJumpBuffer jump_buffer,
			 const void *const key,
			 void **const restrict old_ptr);


/* update state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, old_ptr unset
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no key swap, old_ptr is set
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_update_get_ll(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict grandparent,
		 struct RedBlackNode *const restrict parent,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr);
static bool
rb_update_get_lr(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict grandparent,
		 struct RedBlackNode *const restrict parent,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr);
static bool
rb_update_get_rr(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict grandparent,
		 struct RedBlackNode *const restrict parent,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr);
static bool
rb_update_get_rl(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict grandparent,
		 struct RedBlackNode *const restrict parent,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr);


int
red_black_update_get(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr)
{
	struct RedBlackNode *restrict parent;
	const void *other_key;
	RedBlackUpdateGetNode next_update_get;
	int compare;
	int status;

	struct RedBlackNode *const restrict grandparent = *tree;

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
					next_update_get = (compare < 0)
							? &rb_update_get_ll
							: &rb_update_get_lr;

					(void) next_update_get(tree,
							       grandparent,
							       parent,
							       comparator,
							       factory,
							       jump_buffer,
							       key,
							       old_ptr);
					/* if return, tree must have updated */

				} else {
					*old_ptr = (void *) other_key;
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
					next_update_get = (compare < 0)
							? &rb_update_get_rl
							: &rb_update_get_rr;

					(void) next_update_get(tree,
							       grandparent,
							       parent,
							       comparator,
							       factory,
							       jump_buffer,
							       key,
							       old_ptr);
					/* if return, tree must have updated */

				} else {
					*old_ptr = (void *) other_key;
				}
			}
		}

	} else {
		*old_ptr = (void *) other_key;
	}

	return status;
}



static bool
rb_update_get_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateGetNode next_update_get;
	struct RedBlackNode *restrict node;

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
			*old_ptr = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update_get = (compare < 0)
				? &rb_update_get_ll
				: &rb_update_get_lr;

		status = next_update_get(&grandparent->left,
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
rb_update_get_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateGetNode next_update_get;
	struct RedBlackNode *restrict node;

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
			*old_ptr = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update_get = (compare < 0)
				? &rb_update_get_rl
				: &rb_update_get_rr;

		status = next_update_get(&grandparent->left,
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
rb_update_get_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateGetNode next_update_get;
	struct RedBlackNode *restrict node;

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
			*old_ptr = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update_get = (compare < 0)
				? &rb_update_get_rl
				: &rb_update_get_rr;

		status = next_update_get(&grandparent->right,
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
rb_update_get_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const RedBlackComparator comparator,
	     struct RedBlackNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const void *const key,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	const void *other_key;
	RedBlackUpdateGetNode next_update_get;
	struct RedBlackNode *restrict node;

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
			*old_ptr = (void *) other_key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_update_get = (compare < 0)
				? &rb_update_get_ll
				: &rb_update_get_lr;

		status = next_update_get(&grandparent->right,
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
