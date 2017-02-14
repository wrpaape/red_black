#include "red_black_tree/red_black_treeify.h" /* RedBlackNode */
#include <stddef.h>			      /* NULL */


struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const int length,
		  const bool is_red)
{
	int rem_nodes;
	struct RedBlackNode *restrict node;

	if (length == 0)
		return NULL;

	node = head;

	/* split list in two */
	const int length_left  = length / 2;
	const int length_right = length - length_left - 1;

	/* find root */
	rem_nodes = length_left;

	while (--rem_nodes >= 0)
		node = node->left;


	const bool next_is_red = !is_red;

	node->is_red = is_red;

	struct RedBlackNode *const restrict next_head = node->left;

	node->left  = red_black_treeify(head,
					length_left,
					next_is_red);

	node->right = red_black_treeify(next_head,
					length_right,
					next_is_red);

	return node;
}