#ifndef RED_BLACK_ITOR_TYPES_H_
#define RED_BLACK_ITOR_TYPES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h" /* RedBlackNode */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef struct RedBlackItorNode *restrict
(*RedBlackItorRestoreNode)(struct RedBlackItorNode *const restrict itor_node);

struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
	RedBlackItorRestoreNode restore;
};

struct RedBlackItorCursor {
	struct RedBlackItorNode *restrict *restrict stack;
	struct RedBlackItorNode *restrict path;
};

/* typedef void */
/* (*RedBlackItorDropNode)(struct RedBlackItorCursor *const restrict cursor_ptr, */
/* 			struct RedBlackItorNode *restrict *restrict stack_cursor, */
/* 			struct RedBlackItorNode *restrict path_cursor; */
/* 			struct RedBlackNodeFactory *const restrict factory) */

#endif /* ifndef RED_BLACK_ITOR_TYPES_H_ */
