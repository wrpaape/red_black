#ifndef RED_BLACK_ITOR_TYPES_H_
#define RED_BLACK_ITOR_TYPES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* Node, NodeFactory */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackItorRecoverNode)(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_root,
			   struct RedBlackItorNode *const restrict itor_node);

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

#endif /* ifndef RED_BLACK_ITOR_TYPES_H_ */
