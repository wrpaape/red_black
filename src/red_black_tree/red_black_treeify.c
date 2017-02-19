#include "red_black_tree/red_black_treeify.h" /* RedBlackNode */
#include <stddef.h>			      /* NULL */



unsigned int
rb_treeify(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict head,
	   const unsigned int length)
{
	int rem_nodes;
	struct RedBlackNode *restrict root;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	/* base case -- 1 or 2 nodes left, head is root */
	if (length < 3) {
		/* don't color, returning stack frame will color black */
		head->left = NULL;

		if (length & 1) { /* length == 1 */
			head->right = NULL;

		} else {
			rchild	       = head->right;
			rchild->is_red = true; /* RED leaf */
			rchild->left   = NULL;
			rchild->right  = NULL;
		}

		*tree = head;

		return 1; /* black height (root still not colored) */
	}

	/* always bias split to left (SEE BELOW) */
	const unsigned int left_length  = length / 2;
	const unsigned int right_length = length - left_length - 1;

	/* find root */
	rem_nodes = left_length;
	root      = head;

	do {
		root = root->right;
		--rem_nodes;
	} while (rem_nodes > 0);


	const unsigned int left_black_height  = rb_treeify(&lchild,
							   head,
							   left_length);

	const unsigned int right_black_height = rb_treeify(&rchild,
							   root->right,
							   right_length);

	*tree = root;

	root->left     = lchild;
	lchild->is_red = (left_black_height > right_black_height);

	root->right    = rchild;
	rchild->is_red = false;

	return right_black_height + 1; /* always add 1 to right black height */
}

struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const unsigned int length)
{
	struct RedBlackNode *restrict root;

	if (length == 0)
		return NULL;

	(void) rb_treeify(&root,
			  head,
			  length);

	root->is_red = false; /* root will always be BLACK */

	return root;
}
