#ifndef RED_BLACK_FETCH_H_
#define RED_BLACK_FETCH_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* Node, bool */
#include "red_black_comparator.h" /* Comparator */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_fetch(const struct RedBlackNode *restrict node,
		const RedBlackComparator comparator,
		const void *const key,
		void **const restrict fetch_ptr);

bool
red_black_fetch_min(const struct RedBlackNode *restrict node,
		    void **const restrict fetch_ptr);

bool
red_black_fetch_max(const struct RedBlackNode *restrict node,
		    void **const restrict fetch_ptr);

#endif /* ifndef RED_BLACK_FETCH_H_ */
