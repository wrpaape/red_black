#ifndef RED_BLACK_ITOR_CURSOR_H_
#define RED_BLACK_ITOR_CURSOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_node.h" /* ItorNode */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackItorCursor {
	struct RedBlackItorNode *restrict *restrict stack;
	struct RedBlackItorNode *restrict path;
};

#endif /* ifndef RED_BLACK_ITOR_CURSOR_H_ */
