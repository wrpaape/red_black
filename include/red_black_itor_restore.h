#ifndef RED_BLACK_ITOR_RESTORE_H_
#define RED_BLACK_ITOR_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_types.h"   /* Node, ItorNode, ItorCursor */
#include "red_black_node_factory.h" /* NodeFactory */

void
red_black_itor_restore_root(struct RedBlackItorCursor *const restrict cursor,
			    struct RedBlackItorNode *const restrict itor_root
			    struct RedBlackNodeFactory *const restrict factory);

void
red_black_itor_restore_node(struct RedBlackItorCursor *const restrict cursor,
			    struct RedBlackItorNode *const restrict itor_root
			    struct RedBlackNodeFactory *const restrict factory);
#endif /* ifndef RED_BLACK_ITOR_RESTORE_H_ */
