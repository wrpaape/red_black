#include "red_black_tree/red_black_treeify.h" /* RedBlackNode */
#include <stddef.h>			      /* NULL */



struct RedBlackNode *restrict
rb_treeify(struct RedBlackNode *const restrict head,
	   const unsigned int length,
	   const unsigned int bias)
{
	int rem_nodes;
	bool is_red;
	struct RedBlackNode *restrict node;

	/* base case -- 1 or 2 nodes left */
	if (length < 3) {
		is_red = (bool) (length & 1);

		head->is_red = is_red;
		head->left   = NULL;

		if (is_red) { /* length == 1 */
			head->right = NULL; /* root is RED leaf */

		} else {      /* length == 2 */
			node	     = head->right;
			node->is_red = true; /* make RED leaf */
			node->left   = NULL;
			node->right  = NULL;
		}

		return head;
	}

	/* split list in two, toggle between two methods of determining
	 * 'left_length' to maintain balance:
	 * method 1:
	 *	left_length = length / 2
	 * method 2:
	 *	left_length = (length - 1) / 2
	 *
	 * then set 'right_length':
	 *	right_length = length - left_length - 1 */

	const unsigned int left_length  = (length - bias) / 2; /* bias == 1 or 0 */
	const unsigned int right_length = length - left_length - 1;

	/* find root */
	rem_nodes = left_length;
	node      = head;

	do {
		node = node->right;
		--rem_nodes;
	} while (rem_nodes > 0);

	const unsigned int next_bias = !bias;

	node->is_red = false; /* make black root */
	node->left   = rb_treeify(head,
				  left_length,
				  next_bias);
	node->right  = rb_treeify(node->right,
				  right_length,
				  next_bias);

	return node;
}

struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const unsigned int length)
{
	int rem_nodes;
	struct RedBlackNode *restrict node;

	if (length < 3) {
		if (length & 1) { /* length == 1 */
			head->right = NULL;

		} else if (length == 0) {
			return NULL; /* empty tree */

		} else {	  /* length == 2 */
			node = head->right;
			node->is_red = true; /* make RED leaf */
			node->left   = NULL;
			node->right  = NULL;
		}

		head->is_red = false; /* ensure BLACK root */
		head->left   = NULL;

		return head;
	}

	/* favor right substree on an even-length split */
	const unsigned int left_length  = length / 2;
	const unsigned int right_length = length - left_length - 1;

	/* find root */
	rem_nodes = left_length;
	node      = head;

	do {
		node = node->right;
		--rem_nodes;
	} while (rem_nodes > 0);

	node->is_red = false; /* make black root */

	node->left   = rb_treeify(head,
				  left_length,
				  1); /* favor left on even-length split */
	node->right  = rb_treeify(node->right,
				  right_length,
				  1); /* favor left on even-length split */

	return node;
}
