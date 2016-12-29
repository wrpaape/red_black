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
red_black_insert(struct RedBlackNode *restrict *const restrict root,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	struct RedBlackNode *restrict parent;
	int compare;
	int status;

	struct RedBlackNode *const restrict grandparent = *root;

	if (grandparent == NULL) {
		*root = rb_new_node(allocator,
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
						(void) rb_insert_ll(root,
								    grandparent,
								    parent,
								    comparator,
								    allocator,
								    jump_buffer,
								    key);
					else
						(void) rb_insert_lr(root,
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
						(void) rb_insert_rl(root,
								    grandparent,
								    parent,
								    comparator,
								    allocator,
								    jump_buffer,
								    key);
					else
						(void) rb_insert_rr(root,
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


/* insert correction functions
 *
 *	jump RED_BLACK_JUMP_VALUE_3_TRUE	OK, tree updated
 * or
 *	return					recolor in THIS stack frame
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
rb_insert_correct_ll_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		parent->right = grandparent;

		grandparent->is_red = true;
		grandparent->left   = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_ll_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle->is_red) { /* uncle can't be NULL */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		grandparent->is_red = true;
		grandparent->left   = parent->right;
		parent->right       = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}


static inline void
rb_insert_correct_lr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict child,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left-right, recolor child and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = child;

		child->is_red = false;
		child->left   = parent;
		child->right  = grandparent;

		parent->right = NULL;

		grandparent->is_red = true;
		grandparent->left   = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_lr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict child,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle->is_red) { /* uncle cannot be NULL */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left-right, recolor child and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = child;

		child->is_red = false;
		parent->right = child->left;
		child->left   = parent;

		grandparent->is_red = true;
		grandparent->left   = child->right;
		child->right        = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_rr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		parent->left = grandparent;

		grandparent->is_red = true;
		grandparent->right  = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_rr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle->is_red) { /* uncle can't be NULL */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		grandparent->is_red = true;
		grandparent->right  = parent->left;
		parent->left        = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}


static inline void
rb_insert_correct_rl_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict child,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right-left, recolor child and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = child;

		child->is_red = false;
		child->left   = grandparent;
		child->right  = parent;

		parent->left = NULL;

		grandparent->is_red = true;
		grandparent->right  = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

static inline void
rb_insert_correct_rl_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict child,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle->is_red) { /* uncle cannot be NULL */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right-left, recolor child and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = child;

		child->is_red       = false;
		grandparent->is_red = true;
		grandparent->right  = child->left;
		child->left         = grandparent;

		parent->left = child->right;
		child->right = parent;

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
		rb_insert_correct_ll_bot(tree,
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
			rb_insert_correct_ll_mid(tree,
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
		rb_insert_correct_lr_bot(tree,
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
			rb_insert_correct_lr_mid(tree,
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
	     const void *const key)
{
	struct RedBlackNode *const restrict next = parent->right;

	if (next == NULL) {
		parent->right = rb_new_red_node(key);
		return rb_insert_correct_rr_bot(tree,
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
			return rb_insert_correct_rr_mid(tree,
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
