#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_TREEIFY_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_TREEIFY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* struct RedBlackNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNode *restrict
red_black_treeify(struct RedBlackNode *const restrict head,
		  const bool is_red,
		  const int length);

/* takes a list of ascending nodes (head->left->left ...) with length 'length'
 * and returns a proper red black tree
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_TREEIFY_H_ */
