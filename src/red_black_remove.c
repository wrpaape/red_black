#include "red_black_remove.h"  /* Comparator, Node, NodeFactory, JumpBuffer */
#include "red_black_restore.h" /* restore API */


static inline void
rb_remove_node(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict node,
	       struct RedBlackNodeFactory *const restrict factory,
	       RedBlackJumpBuffer *const restrict jump_buffer,
	       void **const restrict key_ptr)
{
	bool is_red;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	*key_ptr = (void *) node->key;
	is_red   = node->is_red;
	lchild   = node->left;
	rchild   = node->right;

	/* free node */
	rbnf_free(factory,
		  node);

	if (is_red)
		red_black_restore_red(tree, /* always restorable if node is RED */
				      lchild,
				      rchild);
	else if (!red_black_restore_black(tree,
					  lchild,
					  rchild))
		return; /* need to restore */

	RED_BLACK_JUMP_2_TRUE(jump_buffer); /* all done */
}

static inline void
rb_remove_root(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict root,
	       struct RedBlackNodeFactory *const restrict factory,
	       void **const restrict key_ptr)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	*key_ptr = (void *) root->key;
	lchild   = root->left;
	rchild   = root->right;

	/* free node */
	rbnf_free(factory,
		  root);

	(void) red_black_restore_black(tree,
				       lchild,
				       rchild);
}
/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackRemoveNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict parent,
		      struct RedBlackNode *const restrict node,
		      const RedBlackComparator comparator,
		      struct RedBlackNodeFactory *const restrict factory,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key,
		      void **const restrict key_ptr);

/* remove state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_remove_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict lnode,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict key_ptr);
static void
rb_remove_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict rnode,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict key_ptr);

static void
rb_remove_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict lnode,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict key_ptr)
{
	RedBlackRemoveNode next_remove;
	struct RedBlackNode *restrict next_node;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		rb_remove_node(subtree,
			       lnode,
			       factory,
			       jump_buffer,
			       key_ptr);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		if (compare < 0) {
			next_remove = &rb_remove_l;
			next_node   = lnode->left;

		} else {
			next_remove = &rb_remove_r;
			next_node   = lnode->right;
		}

		next_remove(subtree,
			    lnode,
			    next_node,
			    comparator,
			    factory,
			    jump_buffer,
			    key,
			    key_ptr);

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
	    struct RedBlackNode *const restrict rnode,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key,
	    void **const restrict key_ptr)
{
	RedBlackRemoveNode next_remove;
	struct RedBlackNode *restrict next_node;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		rb_remove_node(subtree,
			       rnode,
			       factory,
			       jump_buffer,
			       key_ptr);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		if (compare < 0) {
			next_remove = &rb_remove_l;
			next_node   = rnode->left;

		} else {
			next_remove = &rb_remove_r;
			next_node   = rnode->right;
		}

		next_remove(subtree,
			    rnode,
			    next_node,
			    comparator,
			    factory,
			    jump_buffer,
			    key,
			    key_ptr);

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
		 void **const restrict key_ptr)
{
	int status;
	RedBlackRemoveNode next_remove;
	struct RedBlackNode *restrict next_node;

	struct RedBlackNode *const restrict node = *tree;

	status = (node != NULL);

	if (status) {
		const int compare = comparator(key,
					       node->key);

		status = (compare == 0);

		if (status) {
			rb_remove_root(tree,
				       node,
				       factory,
				       key_ptr);

		} else {
			if (compare < 0) {
				next_remove = &rb_remove_l;
				next_node   = node->left;

			} else {
				next_remove = &rb_remove_r;
				next_node   = node->right;
			}

			next_remove(tree,
				    node,
				    next_node,
				    comparator,
				    factory,
				    jump_buffer,
				    key,
				    key_ptr);

			return 1; /* updated */
		}
	}

	return status;
}
