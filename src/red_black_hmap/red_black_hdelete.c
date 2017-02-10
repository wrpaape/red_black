#include "red_black_hmap/red_black_hdelete.h"	    /* types, JUMP API */
#include "red_black_hmap/red_black_hrestore.h"	    /* hrestore API */
#include "red_black_hmap/red_black_hkey.h"	    /* cmptr */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackHDeleteNode)(struct RedBlackHNode *restrict *const restrict tree,
		       struct RedBlackHNode *const restrict parent,
		       struct RedBlackHNodeFactory *const restrict factory,
		       RedBlackJumpBuffer jump_buffer,
		       const struct RedBlackHKey *const restrict hkey);

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
rb_hdelete_l(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey);
static void
rb_hdelete_r(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey);

static void
rb_hdelete_l(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey)
{
	RedBlackHDeleteNode next_hdelete;

	struct RedBlackHNode *const restrict lnode = parent->left;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackHNode *restrict *const restrict subtree = &parent->left;

	const int compare = red_black_hkey_comparator(hkey,
						      &lnode->hkey);

	if (compare == 0) {
		red_black_hrestore_node(subtree,
					lnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_l_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hdelete = (compare < 0)
			     ? &rb_hdelete_l
			     : &rb_hdelete_r;

		next_hdelete(subtree,
			     lnode,
			     factory,
			     jump_buffer,
			     hkey);

		/* if returned, need to restore */
		red_black_hrestore_l_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_hdelete_r(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey)
{
	RedBlackHDeleteNode next_hdelete;

	struct RedBlackHNode *const restrict rnode = parent->right;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackHNode *restrict *const restrict subtree = &parent->right;

	const int compare = red_black_hkey_comparator(hkey,
						      &rnode->hkey);

	if (compare == 0) {
		red_black_hrestore_node(subtree,
					rnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_r_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hdelete = (compare < 0)
			     ? &rb_hdelete_l
			     : &rb_hdelete_r;

		next_hdelete(subtree,
			     rnode,
			     comparator,
			     factory,
			     jump_buffer,
			     hkey);

		/* if returned, need to restore */
		red_black_hrestore_r_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



int
red_black_hdelete(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey)
{
	int status;
	RedBlackHDeleteNode next_hdelete;

	struct RedBlackHNode *const restrict root = *tree;

	status = (root != NULL);

	if (status) {
		const int compare = red_black_hkey_comparator(hkey,
							      &root->hkey);

		status = (compare == 0);

		if (status) {
			red_black_hrestore_root(tree,
						root,
						factory);

		} else {
			next_hdelete = (compare < 0)
				     ? &rb_hdelete_l
				     : &rb_hdelete_r;

			next_hdelete(tree,
				     root,
				     factory,
				     jump_buffer,
				     hkey);

			return 1; /* updated */
		}
	}

	return status;
}
