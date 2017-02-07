#ifndef RED_BLACK_ASC_ITOR_RESTORE_H_
#define RED_BLACK_ASC_ITOR_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_cursor.h"  /* Node, NodeFactory ItorNode, ItorCursor */
#include "red_black_node_factory.h" /* NodeFactory */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_asc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_root,
			   struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_ASC_ITOR_RESTORE_H_ */
