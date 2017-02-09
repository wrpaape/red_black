#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_FIND_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_FIND_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* Node, bool */
#include "red_black_tree/red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_find(const struct RedBlackNode *restrict node,
	       const RedBlackComparator comparator,
	       const void *const key);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_FIND_H_ */
