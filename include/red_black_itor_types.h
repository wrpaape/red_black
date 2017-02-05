#ifndef RED_BLACK_ITOR_TYPES_H_
#define RED_BLACK_ITOR_TYPES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* Node, NodeFactory, bool */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
/* typedef void */
/* (*RedBlackItorRestoreCursor)(struct RedBlackItorCursor *const restrict cursor, */
/* 			     struct RedBlackItorNode *const restrict itor_root, */
/* 			     struct RedBlackItorNode *const restrict itor_node); */

/* typedef void */
/* (*RedBlackItorRestoreNode)(struct RedBlackItorNode *const restrict itor_root, */
/* 			   struct RedBlackItorNode *const restrict itor_node); */

/* struct RedBlackItorRestoreControlNode { */
/* 	const RedBlackItorRestoreCursor cursor_and_node; */
/* 	const RedBlackItorRestoreCursor only_cursor; */
/* 	const RedBlackItorRestoreNode only_node; */
/* }; */



/* struct RedBlackItorRestoreController { */
/* 	const struct RedBlackItorRestoreNode */
/* }; */

struct RedBlackItorNode;

typedef void
(*RedBlackItorRestoreNode)(struct RedBlackItorNode *const restrict itor_node);

struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
	bool subtree_is_left;
};

struct RedBlackItorCursor {
	struct RedBlackItorNode *restrict *restrict stack;
	struct RedBlackItorNode *restrict path;
};

typedef void
(*RedBlackItorRestore)(struct RedBlackItorCursor *const restrict cursor,
		       struct RedBlackItorNode *const restrict itor_root,
		       struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_ITOR_TYPES_H_ */
