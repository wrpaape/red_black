#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_SET_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_SET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* Node */
#include "red_black_tree/red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_set(struct RedBlackNode *restrict node,
	      const RedBlackComparator comparator,
	      const void *const key);

void
red_black_set_min(struct RedBlackNode *restrict node,
		  const void *const key);

void
red_black_set_max(struct RedBlackNode *restrict node,
		  const void *const key);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_SET_H_ */
