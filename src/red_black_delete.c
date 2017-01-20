#include "red_black_delete.h"	   /* Comparator, Node, NodeFactory, JumpBuffer */
#include "red_black_restore.h"	   /* restore API */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


static inline void
rb_delete_node(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict node,
	       struct RedBlackNodeFactory *const restrict factory,
	       RedBlackJumpBuffer *const restrict jump_buffer)
{
	bool is_red;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	is_red = node->is_red;
	lchild = node->left;
	rchild = node->right;

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
rb_delete_root(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict root,
	       struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	lchild = root->left;
	rchild = root->right;

	/* free node */
	rbnf_free(factory,
		  root);

	(void) red_black_restore_black(tree,
				       lchild,
				       rchild);
}

static inline void
rb_delete_min_node(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *const restrict node,
		   struct RedBlackNodeFactory *const restrict factory,
		   RedBlackJumpBuffer *const restrict jump_buffer)
{
	bool is_red;
	struct RedBlackNode *restrict rchild;

	is_red = node->is_red;
	rchild = node->right;

	/* free node */
	rbnf_free(factory,
		  node);

	*tree = rchild;

	if (!is_red) {
		if (rchild == NULL)
			return; /* still need to restore */

		rchild->is_red = false; /* RED leaf -> restore */
	}

	RED_BLACK_JUMP_2_TRUE(jump_buffer); /* all done */
}

static inline void
rb_delete_max_node(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *const restrict node,
		   struct RedBlackNodeFactory *const restrict factory,
		   RedBlackJumpBuffer *const restrict jump_buffer)
{
	bool is_red;
	struct RedBlackNode *restrict lchild;

	is_red = node->is_red;
	lchild = node->left;

	/* free node */
	rbnf_free(factory,
		  node);

	*tree = lchild;

	if (!is_red) {
		if (lchild == NULL)
			return; /* still need to restore */

		lchild->is_red = false; /* RED leaf -> restore */
	}

	RED_BLACK_JUMP_2_TRUE(jump_buffer); /* all done */
}

static inline void
rb_delete_min_root(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *const restrict root,
		   struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackNode *restrict rchild;

	rchild = root->right;

	/* free node */
	rbnf_free(factory,
		  root);

	*tree = rchild;

	if (rchild != NULL)
		rchild->is_red = false; /* RED leaf -> restore */
}

static inline void
rb_delete_max_root(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNode *const restrict root,
		   struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackNode *restrict lchild;

	lchild = root->left;

	/* free node */
	rbnf_free(factory,
		  root);

	*tree = lchild;

	if (lchild != NULL)
		lchild->is_red = false; /* RED leaf -> restore */
}


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackDeleteNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackNodeFactory *const restrict factory,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key);

/* delete state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);
static void
rb_delete_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);

static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict lnode = parent->left;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		rb_delete_node(subtree,
			       lnode,
			       factory,
			       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_delete = (compare < 0)
			    ? &rb_delete_l
			    : &rb_delete_r;

		next_delete(subtree,
			    lnode,
			    comparator,
			    factory,
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
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict rnode = parent->right;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		rb_delete_node(subtree,
			       rnode,
			       factory,
			       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_delete = (compare < 0)
			    ? &rb_delete_l
			    : &rb_delete_r;

		next_delete(subtree,
			    rnode,
			    comparator,
			    factory,
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
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	int status;
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict root = *tree;

	status = (root != NULL);

	if (status) {
		const int compare = comparator(key,
					       root->key);

		status = (compare == 0);

		if (status) {
			rb_delete_root(tree,
				       root,
				       factory);

		} else {
			next_delete = (compare < 0)
				    ? &rb_delete_l
				    : &rb_delete_r;

			next_delete(tree,
				    root,
				    comparator,
				    factory,
				    jump_buffer,
				    key);

			return 1; /* updated */
		}
	}

	return status;
}


int
red_black_delete_min(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict next;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;

	struct RedBlackNode *const restrict root = *tree;

	if (root == NULL)
		return 0;

	node = root->left;

	if (node == NULL) {
		rb_delete_min_root(tree,
				   root,
				   factory);
		return 1;
	}

	cursor  = &stack[0];
	*cursor = root;

	while (1) {
		next = node->left;

		if (next == NULL)
			break;

		++cursor;
		*cursor = node;

		node = next;
	}

	parent = *cursor;

	rb_delete_min_node(&parent->left,
			   node,
			   jump_buffer);


	/* if returned, need to restore */
	red_black_restore_l_bot(tree,
				parent,
				jump_buffer);

	while (1) {


	}


	return 1;
}




