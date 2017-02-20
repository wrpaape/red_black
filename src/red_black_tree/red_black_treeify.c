#include "red_black_tree/red_black_treeify.h" /* RedBlackNode */
#include <stddef.h>			      /* NULL */



static struct RedBlackNode *restrict
rb_treeify(struct RedBlackNode *const restrict head,
	   const unsigned int length,
	   unsigned int *const restrict black_height)
{
	unsigned int rem_nodes;
	struct RedBlackNode *restrict root;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict next_head;
	struct RedBlackNode *restrict *restrict rchild_ptr;
	unsigned int left_black_height;
	unsigned int right_black_height;

	/* base case -- 1 or 2 nodes left, head is root */
	if (length < 3) {
		/* always color BLACK, won't be retouched unless RIGHT subtree
		 * in that case it will never be changed to RED since '1' is the
		 * minimum returnable black height */
		head->is_red = false;
		head->left   = NULL;
		rchild_ptr   = &head->right;

		if (length & 1) { /* length == 1 */
			*rchild_ptr = NULL;

		} else {          /* length == 2 */
			rchild	       = *rchild_ptr;
			rchild->is_red = true; /* RED leaf */
			rchild->left   = NULL;
			rchild->right  = NULL;
		}

		*black_height = 1; /* (non-NULL) black height */

		return head;
	}

	/* always bias split to right to make better use of cache (SEE BELOW) */
	const unsigned int rem_length   = length - 1;
	const unsigned int left_length  = rem_length / 2;
	const unsigned int right_length = rem_length - left_length;

	/* find root */
	rem_nodes = left_length;
	root      = head;

	do {
		root = root->right;
		--rem_nodes;
	} while (rem_nodes > 0);


	rchild_ptr = &root->right;
	next_head  = *rchild_ptr; /* right subtree list */

	lchild = rb_treeify(head,
			    left_length,
			    &left_black_height);

	rchild = rb_treeify(next_head,
			    right_length,
			    &right_black_height);

	/* black height imbalance can only occur in RIGHT subtree and can have
	 * no more than 1 extra black height
	 *
	 * right_length will only be greater than left_length if length is EVEN,
	 * and in that case it will only be by 1 node */
	rchild->is_red = (right_black_height > left_black_height);

	/* always color BLACK, won't be retouched unless is a RIGHT subtree */
	root->is_red = false;
	root->left   = lchild;
	*rchild_ptr  = rchild;

	*black_height = left_black_height + 1; /* increment black height */

	return root;
}

struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const unsigned int length)
{
	unsigned int black_height; /* unused */

	return (length == 0)
	     ? NULL
	     : rb_treeify(head,
			  length,
			  &black_height);
}
