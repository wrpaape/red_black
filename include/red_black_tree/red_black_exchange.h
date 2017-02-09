#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_EXCHANGE_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_EXCHANGE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* Node, bool */
#include "red_black_tree/red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_exchange(struct RedBlackNode *restrict node,
		   const RedBlackComparator comparator,
		   const void *const key,
		   void **const restrict old_ptr);

bool
red_black_exchange_min(struct RedBlackNode *restrict node,
		       const void *const key,
		       void **const restrict old_ptr);

bool
red_black_exchange_max(struct RedBlackNode *restrict node,
		       const void *const key,
		       void **const restrict old_ptr);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_EXCHANGE_H_ */
