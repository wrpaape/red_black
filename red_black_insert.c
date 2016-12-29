#include "red_black_insert.h"

static inline struct RedBlackNode *
rb_new_node(struct RedBlackAllocator *const restrict allocator,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    const bool is_red)
{
	struct RedBlackNode *const restrict node
	= red_black_allocatore_allocate(allocator,
					jump_buffer);

	node->key    = key;
	node->is_red = is_red;
	node->left   = NULL;
	node->right  = NULL;

	return node;
}



/* insert state machine functions
 *
 * RETURNS
 *
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
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
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

int
red_black_insert(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackNode *restrict parent;
	int compare;
	int status;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		*tree = rb_new_node(allocator,
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
				grandparent->left = rb_new_node(allocator,
								jump_buffer,
								key,
								true); /* RED */

			} else {
				compare = comparator(key,
						     parent->key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					if (compare < 0)
						(void) rb_insert_ll(tree,
								    grandparent,
								    parent,
								    key);
					else
						(void) rb_insert_lr(tree,
								    grandparent,
								    parent,
								    key);
					/* if return, tree must have updated */
				}
			}


		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = rb_new_node(allocator,
								 jump_buffer,
								 key,
								 true);

			} else {
				compare = comparator(key,
						     parent->key);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					if (compare < 0)
						(void) rb_insert_rl(tree,
								    grandparent,
								    parent,
								    key);
					else
						(void) rb_insert_rr(tree,
								    grandparent,
								    parent,
								    key);
					/* if return, tree must have updated */
				}
			}
		}
	}

	return status;
}

static inline void
rb_rotate_left(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict grandparent,
	       struct RedBlackNode *const restrict rparent)
{
	*tree = rparent;

	grandparent->right = rparent->left;
	rparent->left      = grandparent;
}

static inline void
rb_rotate_right(struct RedBlackNode *restrict *const restrict tree,
		struct RedBlackNode *const restrict grandparent,
		struct RedBlackNode *const restrict lparent)
{
	*tree = lparent;

	grandparent->left = lparent->right;
	lparent->right    = grandparent;
}

static inline void
rb_rotate_left_right(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict lparent,
		     struct RedBlackNode *const restrict lrchild)
{
	*tree = lrchild;

	lparent->right = lrchild->left;
	lrchild->left  = lparent;

	grandparent->left = lrchild->right;
	lrchild->right    = grandparent;
}

static inline void
rb_rotate_right_left(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict rparent,
		     struct RedBlackNode *const restrict rlchild)
{
	*tree = rlchild;

	grandparent->right = rlchild->left;
	rlchild->left      = grandparent;

	rparent->left  = rlchild->right;
	rlchild->right = rparent;
}



/* insert correction functions
 *
 *	jump RED_BLACK_JUMP_VALUE_3_TRUE	OK, tree updated
 * or
 *	return					correct previous state frame
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
rb_insert_correct_ll(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		/* needs to be red for bubble up or rotation -> jump */
		grandparent->is_red = true;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to correct 2 frames previous */
		}

		rb_rotate_right(tree,
				grandparent,
				parent);
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_lr(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     struct RedBlackNode *const restrict child,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		/* needs to be red for bubble up or rotation -> jump */
		grandparent->is_red = true;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to correct 2 frames previous */
		}

		child->is_red = false;

		rb_rotate_left_right(tree,
				     grandparent,
				     parent,
				     child);
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_rr(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{

	if (parent->is_red) {
		parent->is_red = false;

		/* needs to be red for bubble up or rotation -> jump */
		grandparent->is_red = true;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to correct 2 frames previous */
		}

		rb_rotate_left(tree,
			       grandparent,
			       parent);
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_rl(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     struct RedBlackNode *const restrict child,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		/* needs to be red for bubble up or rotation -> jump */
		grandparent->is_red = true;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to correct 2 frames previous */
		}

		child->is_red = false;

		rb_rotate_right_left(tree,
				     grandparent,
				     parent,
				     child);
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
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
	struct RedBlackNode *restrict *restrict next_tree;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		parent->left = rb_new_node(allocator,
					   jump_buffer,
					   key,
					   true); /* RED */

		/* need to correct */
		rb_insert_correct_ll(tree,
				     grandparent,
				     parent,
				     jump_buffer);

	} else {
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_tree = &grandparent->left;

		status = (compare < 0)
		       ? rb_insert_ll(next_tree,
				      parent,
				      node,
				      comparator,
				      allocator,
				      jump_buffer,
				      key)
		       : rb_insert_lr(next_tree,
				      parent,
				      node,
				      comparator,
				      allocator,
				      jump_buffer,
				      key);

		if (status) /* need to correct */
			rb_insert_correct_ll(tree,
					     grandparent,
					     parent,
					     jump_buffer);
	}

	/* if had to correct in this frame and didn't jump, need to correct in
	 * the previous-previous frame, otherwise need to correct in previous frame */
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
	struct RedBlackNode *restrict *restrict next_tree;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		node = rb_new_node(allocator,
				   jump_buffer,
				   key,
				   true); /* RED */

		parent->right = node;

		/* need to correct */
		rb_insert_correct_lr(tree,
				     grandparent,
				     parent,
				     node,
				     jump_buffer);
	} else {
		compare = comparator(key,
				     node->key);

		if (compare == 0)
			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */

		next_tree = &grandparent->left;

		status = (compare < 0)
		       ? rb_insert_rl(next_tree,
				      parent,
				      node,
				      comparator,
				      allocator,
				      jump_buffer,
				      key)
		       : rb_insert_rr(next_tree,
				      parent,
				      node,
				      comparator,
				      allocator,
				      jump_buffer,
				      key);

		if (status) /* need to correct */
			rb_insert_correct_lr(tree,
					     grandparent,
					     parent,
					     parent->right,
					     jump_buffer);
	}

	/* if had to correct in this frame and didn't jump, need to recolor in
	 * previous frame
	 * if recolored in this frame, need to correct in previous frame */
	return !status;
}

static bool
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const void *const key)
{
	struct RedBlackNode *const restrict next = parent->right;

	if (next == NULL) {
		parent->right = rb_new_red_node(key);
		return rb_insert_correct_rr(tree,
					    grandparent,
					    parent);
	}

	const int compare = comparator(key,
				       next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->right;

		const int state
		= (compare < 0)
		? rb_insert_rl(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_rr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->is_red = true;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_rr(tree,
						    grandparent,
						    parent);
		}
	}

	return CORRECT_DONE;
}

static bool
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const void *const key)
{
	struct RedBlackNode *const restrict next = parent->left;

	if (next == NULL) {
		struct RedBlackNode *const restrict node = rb_new_red_node(key);
		parent->left = node;
		return rb_insert_correct_rl(tree,
					    grandparent,
					    parent,
					    node);
	}

	const int compare = comparator(key,
				       next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->right;

		const int state
		= (compare < 0)
		? rb_insert_ll(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_lr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->is_red = true;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_rl(tree,
						    grandparent,
						    parent,
						    parent->left);
		}
	}

	return CORRECT_DONE;
}
