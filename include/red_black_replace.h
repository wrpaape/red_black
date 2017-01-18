#ifndef RED_BLACK_REPLACE_H_
#define RED_BLACK_REPLACE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* Node, bool */
#include "red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_replace(struct RedBlackNode *restrict node,
		  const RedBlackComparator comparator,
		  const void *const key,
		  void **const restrict old_ptr);

#endif /* ifndef RED_BLACK_REPLACE_H_ */
