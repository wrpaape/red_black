#include "red_black_hmap/red_black_hupdate_get.h" /* types, JUMP API */
#include "red_black_hmap/red_black_hcorrect.h"    /* post-insertion correction */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
(*RedBlackHUpdateGetHNode)(struct RedBlackHNode *restrict *const restrict tree,
			   struct RedBlackHNode *const restrict grandparent,
			   struct RedBlackHNode *const restrict parent,
			   struct RedBlackHNodeFactory *const restrict factory,
			   RedBlackJumpBuffer jump_buffer,
			   const struct RedBlackHKey *const restrict hkey,
			   void **const restrict old_ptr);


/* update state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, old_ptr unset
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, successful hkey swap, old_ptr is set
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * RETURNS
 *	true	need to correct (check if can rotate, recolor) in THIS frame
 *	false	need to recolor parent in THIS frame, correct in PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
static bool
rb_hupdate_get_ll(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNode *const restrict grandparent,
		  struct RedBlackHNode *const restrict parent,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict old_ptr);
static bool
rb_hupdate_get_lr(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNode *const restrict grandparent,
		  struct RedBlackHNode *const restrict parent,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict old_ptr);
static bool
rb_hupdate_get_rr(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNode *const restrict grandparent,
		  struct RedBlackHNode *const restrict parent,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict old_ptr);
static bool
rb_hupdate_get_rl(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNode *const restrict grandparent,
		  struct RedBlackHNode *const restrict parent,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict old_ptr);


int
red_black_hupdate_get(struct RedBlackHNode *restrict *const restrict tree,
		      struct RedBlackHNodeFactory *const restrict factory,
		      RedBlackJumpBuffer jump_buffer,
		      const struct RedBlackHKey *const restrict hkey,
		      void **const restrict old_ptr)
{
	struct RedBlackHNode *restrict parent;
	RedBlackHUpdateGetHNode next_hupdate_get;
	int compare;
	int status;

	struct RedBlackHNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		*tree = rbhnf_new(factory,
				  jump_buffer,
				  hkey,
				  false); /* BLACK */
		return 1; /* tree updated */
	}

	compare = red_black_hkey_comparator(hkey,
					    &grandparent->hkey);

	status = (compare != 0); /* 1, 0 */

	if (status) {
		if (compare < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = rbhnf_new(factory,
							      jump_buffer,
							      hkey,
							      true); /* RED */

			} else {
				compare
				= red_black_hkey_comparator(hkey,
							    &parent->hkey);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_hupdate_get = (compare < 0)
							 ? &rb_hupdate_get_ll
							 : &rb_hupdate_get_lr;

					(void) next_hupdate_get(tree,
								grandparent,
								parent,
								factory,
								jump_buffer,
								hkey,
								old_ptr);
					/* if return, tree must have updated */

				} else {
					*old_ptr = (void *) parent->hkey.key;
				}
			}

		} else {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = rbhnf_new(factory,
							       jump_buffer,
							       hkey,
							       true); /* RED */

			} else {
				compare
				= red_black_hkey_comparator(hkey,
							    &parent->hkey);

				status = (compare != 0); /* 1, 0 */

				if (status) {
					next_hupdate_get = (compare < 0)
							 ? &rb_hupdate_get_rl
							 : &rb_hupdate_get_rr;

					(void) next_hupdate_get(tree,
								grandparent,
								parent,
								factory,
								jump_buffer,
								hkey,
								old_ptr);
					/* if return, tree must have updated */

				} else {
					*old_ptr = (void *) parent->hkey.key;
				}
			}
		}

	} else {
		*old_ptr = (void *) grandparent->hkey.key;
	}

	return status;
}



static bool
rb_hupdate_get_ll(struct RedBlackHNode *restrict *const restrict tree,
	      struct RedBlackHNode *const restrict grandparent,
	      struct RedBlackHNode *const restrict parent,
	      struct RedBlackHNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer,
	      const struct RedBlackHKey *const restrict hkey,
	      void **const restrict old_ptr)
{
	bool status;
	int compare;
	RedBlackHUpdateGetHNode next_hupdate_get;
	struct RedBlackHNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		parent->left = rbhnf_new(factory,
					 jump_buffer,
					 hkey,
					 true); /* RED */

		/* need to correct */
		red_black_hcorrect_ll_bot(tree,
					  grandparent,
					  parent,
					  jump_buffer);

	} else {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			*old_ptr = (void *) node->hkey.key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_hupdate_get = (compare < 0)
				 ? &rb_hupdate_get_ll
				 : &rb_hupdate_get_lr;

		status = next_hupdate_get(&grandparent->left,
					  parent,
					  node,
					  factory,
					  jump_buffer,
					  hkey,
					  old_ptr);

		if (status) /* need to correct */
			red_black_hcorrect_ll_mid(tree,
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
rb_hupdate_get_lr(struct RedBlackHNode *restrict *const restrict tree,
	     struct RedBlackHNode *const restrict grandparent,
	     struct RedBlackHNode *const restrict parent,
	     struct RedBlackHNodeFactory *const restrict factory,
	     RedBlackJumpBuffer jump_buffer,
	     const struct RedBlackHKey *const restrict hkey,
	     void **const restrict old_ptr)
{
	bool status;
	int compare;
	RedBlackHUpdateGetHNode next_hupdate_get;
	struct RedBlackHNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		node = rbhnf_new(factory,
				 jump_buffer,
				 hkey,
				 true); /* RED */

		parent->right = node;

		/* need to correct */
		red_black_hcorrect_lr_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			*old_ptr = (void *) node->hkey.key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_hupdate_get = (compare < 0)
				 ? &rb_hupdate_get_rl
				 : &rb_hupdate_get_rr;

		status = next_hupdate_get(&grandparent->left,
					  parent,
					  node,
					  factory,
					  jump_buffer,
					  hkey,
					  old_ptr);

		if (status) /* need to correct */
			red_black_hcorrect_lr_mid(tree,
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
rb_hupdate_get_rr(struct RedBlackHNode *restrict *const restrict tree,
	      struct RedBlackHNode *const restrict grandparent,
	      struct RedBlackHNode *const restrict parent,
	      struct RedBlackHNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer,
	      const struct RedBlackHKey *const restrict hkey,
	      void **const restrict old_ptr)
{
	bool status;
	int compare;
	RedBlackHUpdateGetHNode next_hupdate_get;
	struct RedBlackHNode *restrict node;

	node = parent->right;

	status = (node == NULL);

	if (status) {
		parent->right = rbhnf_new(factory,
					  jump_buffer,
					  hkey,
					  true); /* RED */

		/* need to correct */
		red_black_hcorrect_rr_bot(tree,
					 grandparent,
					 parent,
					 jump_buffer);

	} else {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			*old_ptr = (void *) node->hkey.key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_hupdate_get = (compare < 0)
				 ? &rb_hupdate_get_rl
				 : &rb_hupdate_get_rr;

		status = next_hupdate_get(&grandparent->right,
					  parent,
					  node,
					  factory,
					  jump_buffer,
					  hkey,
					  old_ptr);

		if (status) /* need to correct */
			red_black_hcorrect_rr_mid(tree,
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
rb_hupdate_get_rl(struct RedBlackHNode *restrict *const restrict tree,
	      struct RedBlackHNode *const restrict grandparent,
	      struct RedBlackHNode *const restrict parent,
	      struct RedBlackHNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer,
	      const struct RedBlackHKey *const restrict hkey,
	      void **const restrict old_ptr)
{
	bool status;
	int compare;
	RedBlackHUpdateGetHNode next_hupdate_get;
	struct RedBlackHNode *restrict node;

	node = parent->left;

	status = (node == NULL);

	if (status) {
		node = rbhnf_new(factory,
				 jump_buffer,
				 hkey,
				 true); /* RED */

		parent->left = node;

		/* need to correct */
		red_black_hcorrect_rl_bot(tree,
					 grandparent,
					 parent,
					 node,
					 jump_buffer);

	} else {
		compare = red_black_hkey_comparator(hkey,
						    &node->hkey);

		if (compare == 0) {
			*old_ptr = (void *) node->hkey.key;

			RED_BLACK_JUMP_3_FALSE(jump_buffer); /* all done */
		}

		next_hupdate_get = (compare < 0)
				 ? &rb_hupdate_get_ll
				 : &rb_hupdate_get_lr;

		status = next_hupdate_get(&grandparent->right,
					  parent,
					  node,
					  factory,
					  jump_buffer,
					  hkey,
					  old_ptr);

		if (status) /* need to correct */
			red_black_hcorrect_rl_mid(tree,
						  grandparent,
						  parent,
						  parent->left,
						  jump_buffer);
		else /* need to recolor */
			parent->is_red = true;
	}

	/* if had to correct and didn't jump, need to recolor in previous frame,
	 * otherwise if had to recolor, need to correct in previous frame */
	return !status;
}
