#include "red_black_hmap/red_black_hpluck.h"	    /* types, JUMP API */
#include "red_black_hmap/red_black_hrestore.h"	    /* hrestore API */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackHPluckHNode)(struct RedBlackHNode *restrict *const restrict tree,
		       struct RedBlackHNode *const restrict parent,
		       struct RedBlackHNodeFactory *const restrict factory,
		       RedBlackJumpBuffer jump_buffer,
		       const struct RedBlackHKey *const restrict hkey,
		       void **const restrict pluck_ptr);

/* pluck state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_hpluck_l(struct RedBlackHNode *restrict *const restrict tree,
	    struct RedBlackHNode *const restrict parent,
	    struct RedBlackHNodeFactory *const restrict factory,
	    RedBlackJumpBuffer jump_buffer,
	    const struct RedBlackHKey *const restrict hkey,
	    void **const restrict pluck_ptr);
static void
rb_hpluck_r(struct RedBlackHNode *restrict *const restrict tree,
	    struct RedBlackHNode *const restrict parent,
	    struct RedBlackHNodeFactory *const restrict factory,
	    RedBlackJumpBuffer jump_buffer,
	    const struct RedBlackHKey *const restrict hkey,
	    void **const restrict pluck_ptr);

static void
rb_hpluck_l(struct RedBlackHNode *restrict *const restrict tree,
	    struct RedBlackHNode *const restrict parent,
	    struct RedBlackHNodeFactory *const restrict factory,
	    RedBlackJumpBuffer jump_buffer,
	    const struct RedBlackHKey *const restrict hkey,
	    void **const restrict pluck_ptr)
{
	RedBlackHPluckHNode next_hpluck;

	struct RedBlackHNode *restrict *const restrict subtree = &parent->left;
	struct RedBlackHNode *const restrict lnode	       = *subtree;

	const int compare = red_black_hkey_comparator(hkey,
						      &lnode->hkey);

	if (compare == 0) {
		*pluck_ptr = (void *) lnode->hkey.key;

		red_black_hrestore_node(subtree,
					lnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_l_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hpluck = (compare < 0)
			    ? &rb_hpluck_l
			    : &rb_hpluck_r;

		next_hpluck(subtree,
			    lnode,
			    factory,
			    jump_buffer,
			    hkey,
			    pluck_ptr);

		/* if returned, need to restore */
		red_black_hrestore_l_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_hpluck_r(struct RedBlackHNode *restrict *const restrict tree,
	    struct RedBlackHNode *const restrict parent,
	    struct RedBlackHNodeFactory *const restrict factory,
	    RedBlackJumpBuffer jump_buffer,
	    const struct RedBlackHKey *const restrict hkey,
	    void **const restrict pluck_ptr)
{
	RedBlackHPluckHNode next_hpluck;

	struct RedBlackHNode *restrict *const restrict subtree = &parent->right;
	struct RedBlackHNode *const restrict rnode	       = *subtree;

	const int compare = red_black_hkey_comparator(hkey,
						      &rnode->hkey);

	if (compare == 0) {
		*pluck_ptr = (void *) rnode->hkey.key;

		red_black_hrestore_node(subtree,
					rnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_r_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hpluck = (compare < 0)
			    ? &rb_hpluck_l
			    : &rb_hpluck_r;

		next_hpluck(subtree,
			    rnode,
			    factory,
			    jump_buffer,
			    hkey,
			    pluck_ptr);

		/* if returned, need to restore */
		red_black_hrestore_r_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



void
red_black_hpluck(struct RedBlackHNode *restrict *const restrict tree,
		 struct RedBlackHNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const struct RedBlackHKey *const restrict hkey,
		 void **const restrict pluck_ptr)
{
	RedBlackHPluckHNode next_hpluck;

	struct RedBlackHNode *const restrict root = *tree;

	const int compare = red_black_hkey_comparator(hkey,
						      &root->hkey);

	if (compare == 0) {
		*pluck_ptr = (void *) root->hkey.key;

		red_black_hrestore_root(tree,
					root,
					factory);

	} else {
		next_hpluck = (compare < 0)
			    ? &rb_hpluck_l
			    : &rb_hpluck_r;

		next_hpluck(tree,
			    root,
			    factory,
			    jump_buffer,
			    hkey,
			    pluck_ptr);
	}
}
