#include "red_black_delete.h"  /* Comparator, Node, Allocator, JumpBuffer */
#include "red_black_restore.h" /* restore API */


static inline void
rb_delete_node(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict node,
	       struct RedBlackAllocator *const restrict allocator,
	       RedBlackJumpBuffer *const restrict jump_buffer)
{
	bool is_red;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	is_red = node->is_red;
	lchild = node->left;
	rchild = node->right;

	/* free node */
	red_black_allocator_free(allocator,
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
rb_delete_root(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict root,
	       struct RedBlackAllocator *const restrict allocator)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	lchild = root->left;
	rchild = root->right;

	/* free node */
	red_black_allocator_free(allocator,
				 root);

	(void) red_black_restore_black(tree,
				       lchild,
				       rchild);
}
/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackDeleteNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict parent,
		      struct RedBlackNode *const restrict node,
		      const RedBlackComparator comparator,
		      struct RedBlackAllocator *const restrict allocator,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key);

/* delete state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict lnode,
	    const RedBlackComparator comparator,
	    struct RedBlackAllocator *const restrict allocator,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);
static void
rb_delete_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict rnode,
	    const RedBlackComparator comparator,
	    struct RedBlackAllocator *const restrict allocator,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);

static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict lnode,
	    const RedBlackComparator comparator,
	    struct RedBlackAllocator *const restrict allocator,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;
	struct RedBlackNode *restrict next_node;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		rb_delete_node(subtree,
			       lnode,
			       allocator,
			       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		if (compare < 0) {
			next_delete = &rb_delete_l;
			next_node   = lnode->left;

		} else {
			next_delete = &rb_delete_r;
			next_node   = lnode->right;
		}

		next_delete(subtree,
			    lnode,
			    next_node,
			    comparator,
			    allocator,
			    jump_buffer,
			    key);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_delete_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict rnode,
	    const RedBlackComparator comparator,
	    struct RedBlackAllocator *const restrict allocator,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;
	struct RedBlackNode *restrict next_node;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		rb_delete_node(subtree,
			       rnode,
			       allocator,
			       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		if (compare < 0) {
			next_delete = &rb_delete_l;
			next_node   = rnode->left;

		} else {
			next_delete = &rb_delete_r;
			next_node   = rnode->right;
		}

		next_delete(subtree,
			    rnode,
			    next_node,
			    comparator,
			    allocator,
			    jump_buffer,
			    key);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



int
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	int status;
	RedBlackDeleteNode next_delete;
	struct RedBlackNode *restrict next_node;

	struct RedBlackNode *const restrict node = *tree;

	status = (node != NULL);

	if (status) {
		const int compare = comparator(key,
					       node->key);

		status = (compare == 0);

		if (status) {
			rb_delete_root(tree,
				       node,
				       allocator);

		} else {
			if (compare < 0) {
				next_delete = &rb_delete_l;
				next_node   = node->left;

			} else {
				next_delete = &rb_delete_r;
				next_node   = node->right;
			}

			next_delete(tree,
				    node,
				    next_node,
				    comparator,
				    allocator,
				    jump_buffer,
				    key);

			return 1; /* updated */
		}
	}

	return status;
}
