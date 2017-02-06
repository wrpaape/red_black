#ifndef RED_BLACK_ASC_ITOR_RESTORE_H_
#define RED_BLACK_ASC_ITOR_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_types.h" /* Node, NodeFactory ItorNode, ItorCursor */

void
red_black_asc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_root,
			   struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_ASC_ITOR_RESTORE_H_ */
