#ifndef RED_BLACK_FIND_H_
#define RED_BLACK_FIND_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_utils.h" /* Key, RedBlackNode, exit_on_* */

bool
red_black_find(const struct RedBlackNode *restrict node,
	       const struct Key *const restrict key);
#endif /* ifndef RED_BLACK_FIND_H_ */
