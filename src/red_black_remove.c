#include "red_black_remove.h"  /* Comparator, Node, NodeFactory, JumpBuffer */
#include "red_black_restore.h" /* restore API */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackRemoveNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackNodeFactory *const restrict factory,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key,
		      void **const restrict remove_ptr);

/* remove state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_remove_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict remove_ptr);
static void
rb_remove_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict remove_ptr);

static void
rb_remove_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict remove_ptr)
{
	RedBlackRemoveNode next_remove;

	struct RedBlackNode *const restrict lnode = parent->left;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		*remove_ptr = (void *) lnode->key;

		red_black_restore_node(subtree,
				       lnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_remove = (compare < 0)
			    ? &rb_remove_l
			    : &rb_remove_r;

		next_remove(subtree,
			    lnode,
			    comparator,
			    factory,
			    jump_buffer,
			    key,
			    remove_ptr);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_remove_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict remove_ptr)
{
	RedBlackRemoveNode next_remove;

	struct RedBlackNode *const restrict rnode = parent->right;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		*remove_ptr = (void *) rnode->key;

		red_black_restore_node(subtree,
				       rnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_remove = (compare < 0)
			    ? &rb_remove_l
			    : &rb_remove_r;

		next_remove(subtree,
			    rnode,
			    comparator,
			    factory,
			    jump_buffer,
			    key,
			    remove_ptr);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



int
red_black_remove(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key,
		 void **const restrict remove_ptr)
{
	int status;
	RedBlackRemoveNode next_remove;

	struct RedBlackNode *const restrict root = *tree;

	status = (root != NULL);

	if (status) {
		const int compare = comparator(key,
					       root->key);

		status = (compare == 0);

		if (status) {
			*remove_ptr = (void *) root->key;

			red_black_restore_root(tree,
					       root,
					       factory);

		} else {
			next_remove = (compare < 0)
				    ? &rb_remove_l
				    : &rb_remove_r;

			next_remove(tree,
				    root,
				    comparator,
				    factory,
				    jump_buffer,
				    key,
				    remove_ptr);

			return 1; /* updated */
		}
	}

	return status;
}
