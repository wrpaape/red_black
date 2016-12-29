#ifndef RED_BLACK_DELETE_H_
#define RED_BLACK_DELETE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_utils.h" /* Key, RedBlackNode, exit_on_* */

void
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const struct Key *const restrict key);
#endif /* ifndef RED_BLACK_DELETE_H_ */
