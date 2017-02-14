#include "red_black_hmap/red_black_hremove.h"	    /* types, JUMP API */
#include "red_black_hmap/red_black_hrestore.h"	    /* hrestore API */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackHRemoveHNode)(struct RedBlackHNode *restrict *const restrict tree,
			struct RedBlackHNode *const restrict parent,
			struct RedBlackHNodeFactory *const restrict factory,
			RedBlackJumpBuffer jump_buffer,
			const struct RedBlackHKey *const restrict hkey,
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
rb_hremove_l(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey,
	     void **const restrict remove_ptr);
static void
rb_hremove_r(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey,
	     void **const restrict remove_ptr);

static void
rb_hremove_l(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey,
	     void **const restrict remove_ptr)
{
	RedBlackHRemoveHNode next_hremove;

	struct RedBlackHNode *const restrict lnode = parent->left;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackHNode *restrict *const restrict subtree = &parent->left;

	const int compare = red_black_hkey_comparator(hkey,
						      &lnode->hkey);

	if (compare == 0) {
		*remove_ptr = (void *) lnode->hkey.key;

		red_black_hrestore_node(subtree,
					lnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_l_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hremove = (compare < 0)
			     ? &rb_hremove_l
			     : &rb_hremove_r;

		next_hremove(subtree,
			     lnode,
			     factory,
			     jump_buffer,
			     hkey,
			     remove_ptr);

		/* if returned, need to restore */
		red_black_hrestore_l_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_hremove_r(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey,
	     void **const restrict remove_ptr)
{
	RedBlackHRemoveHNode next_hremove;

	struct RedBlackHNode *const restrict rnode = parent->right;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackHNode *restrict *const restrict subtree = &parent->right;

	const int compare = red_black_hkey_comparator(hkey,
						      &rnode->hkey);

	if (compare == 0) {
		*remove_ptr = (void *) rnode->hkey.key;

		red_black_hrestore_node(subtree,
					rnode,
					factory,
					jump_buffer);

		/* if returned, need to restore */
		red_black_hrestore_r_bot(tree,
					 parent,
					 jump_buffer);

	} else {
		next_hremove = (compare < 0)
			     ? &rb_hremove_l
			     : &rb_hremove_r;

		next_hremove(subtree,
			     rnode,
			     factory,
			     jump_buffer,
			     hkey,
			     remove_ptr);

		/* if returned, need to restore */
		red_black_hrestore_r_mid(tree,
					 parent,
					 jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



int
red_black_hremove(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict remove_ptr)
{
	int status;
	RedBlackHRemoveHNode next_hremove;

	struct RedBlackHNode *const restrict root = *tree;

	status = (root != NULL);

	if (status) {
		const int compare = red_black_hkey_comparator(hkey,
							      &root->hkey);

		status = (compare == 0);

		if (status) {
			*remove_ptr = (void *) root->hkey.key;

			red_black_hrestore_root(tree,
					       root,
					       factory);

		} else {
			next_hremove = (compare < 0)
				     ? &rb_hremove_l
				     : &rb_hremove_r;

			next_hremove(tree,
				     root,
				     factory,
				     jump_buffer,
				     hkey,
				     remove_ptr);

			return 1; /* updated */
		}
	}

	return status;
}
