#include "red_black_tree/red_black_treeify.h" /* RedBlackNode */
#include <stddef.h>			      /* NULL */

#include <stdio.h>
#define DEBUG(...) printf(__VA_ARGS__), fflush(stdout)

struct RedBlackNode *restrict
rb_treeify(struct RedBlackNode *const restrict head,
	   const int length,
	   const int bias)
{
	int rem_nodes;
	struct RedBlackNode *restrict node;

	DEBUG("bias: %d, length: %d\n", bias, length);

	/* base case -- 1 or 2 nodes left */
	if (length < 3) {
		DEBUG("DOIN IT\n");
		head->is_red = false; /* BLACK */

		if (length & 1) { /* length == 1 */
			head->left  = NULL;
			head->right = NULL;
		} else {          /* length == 2 */
			node = head->right;

			node->is_red = true; /* make RED leaf */
			node->left   = NULL;
			node->right  = NULL;
		}
		DEBUG("DONE\n");

		return head;
	}

	/* split list in two, toggle between two methods of determining
	 * 'left_length' to achieve better balance:
	 * method 1:
	 *	left_length = length / 2
	 * method 2:
	 *	left_length = (length - 1) / 2
	 *
	 * then set 'right_length':
	 *	right_length = length - left_length - 1 */

	const int left_length  = (length - bias) / 2; /* bias = 1 or 0 */
	const int right_length = length - left_length - 1;

	/* find root */
	rem_nodes = left_length;
	node      = head;

	DEBUG("left_length: %d, right_length: %d\n", left_length, right_length);

	do {
		DEBUG("rem_nodes: %d\n", rem_nodes);
		node = node->right;
		--rem_nodes;
	} while (rem_nodes > 0);

	const int next_bias = !bias;

	node->is_red = false; /* make black node */

	/* struct RedBlackNode *const restrict next_head = node->right; */

	node->left  = rb_treeify(head,
				 left_length,
				 next_bias);

	node->right = rb_treeify(node->right,
				 right_length,
				 next_bias);

	return node;
}

struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const int length)
{
	return (length > 0)
	     ? rb_treeify(head,
			  length,
			  0)
	     : NULL;
}
