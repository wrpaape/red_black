#ifndef RED_BLACK_GET_H_
#define RED_BLACK_GET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* Node */
#include "red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void *
red_black_get(const struct RedBlackNode *restrict node,
	      const RedBlackComparator comparator,
	      const void *const key);

void *
red_black_get_min(const struct RedBlackNode *restrict node);

void *
red_black_get_max(const struct RedBlackNode *restrict node);

#endif /* ifndef RED_BLACK_GET_H_ */
