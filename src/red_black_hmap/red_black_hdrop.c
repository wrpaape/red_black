#include "red_black_hmap/red_black_hdrop.h"	    /* types, JUMP API */
#include "red_black_hmap/red_black_hrestore.h"	    /* hrestore API */
#include "red_black_hmap/red_black_hkey.h"	    /* cmptr */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackHDropNode)(struct RedBlackHNode *restrict *const restrict tree,
		     struct RedBlackHNode *const restrict parent,
		     struct RedBlackHNodeFactory *const restrict factory,
		     RedBlackJumpBuffer jump_buffer,
		     const void *const hkey);

/* drop state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_hdrop_l(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict parent,
	   struct RedBlackHNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const hkey);
static void
rb_hdrop_r(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict parent,
	   struct RedBlackHNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const hkey);

static void
rb_hdrop_l(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict parent,
	   struct RedBlackHNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const hkey)
{
	RedBlackHDropNode next_hdrop;

	struct RedBlackHNode *restrict *const restrict subtree = &parent->left;
	struct RedBlackHNode *const restrict lnode	       = *subtree;

	const int compare = red_black_hkey_comparator(key,
						      &lnode->hkey);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       lnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_hdrop = (compare < 0)
			   ? &rb_hdrop_l
			   : &rb_hdrop_r;

		next_hdrop(subtree,
			   lnode,
			   factory,
			   jump_buffer,
			   hkey);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_hdrop_r(struct RedBlackHNode *restrict *const restrict tree,
	   struct RedBlackHNode *const restrict parent,
	   struct RedBlackHNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const hkey)
{
	RedBlackHDropNode next_hdrop;

	struct RedBlackHNode *restrict *const restrict subtree = &parent->right;
	struct RedBlackHNode *const restrict rnode	       = *subtree;

	const int compare = red_black_hkey_comparator(key,
						      &rnode->hkey);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       rnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_hdrop = (compare < 0)
			   ? &rb_hdrop_l
			   : &rb_hdrop_r;

		next_hdrop(subtree,
			   rnode,
			   factory,
			   jump_buffer,
			   hkey);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



void
red_black_hdrop(struct RedBlackHNode *restrict *const restrict tree,
		struct RedBlackHNodeFactory *const restrict factory,
		RedBlackJumpBuffer jump_buffer,
		const void *const hkey)
{
	RedBlackHDropNode next_hdrop;
	struct RedBlackHNode *const restrict root = *tree;

	const int compare = red_black_hkey_comparator(key,
						      &root->hkey);

	if (compare == 0) {
		red_black_restore_root(tree,
				       root,
				       factory);

	} else {
		next_hdrop = (compare < 0)
			   ? &rb_hdrop_l
			   : &rb_hdrop_r;

		next_hdrop(tree,
			   root,
			   factory,
			   jump_buffer,
			   hkey);
	}
}
