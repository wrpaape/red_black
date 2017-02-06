#ifndef RED_BLACK_ITOR_TYPES_H_
#define RED_BLACK_ITOR_TYPES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* Node, NodeFactory, bool */


/* parent_info macros
 * ────────────────────────────────────────────────────────────────────────── */
/* bit 0          ==     0/1    | bit 1 ==            0/1
 * parent->is_red == false/true | node  == parent->left/right
 */
#define RBI_PARENT_INFO_NONE	    -1
#define RBI_PARENT_INFO_BLACK_LEFT   0
#define RBI_PARENT_INFO_RED_LEFT     1
#define RBI_PARENT_INFO_BLACK_RIGHT  2
#define RBI_PARENT_INFO_RED_RIGHT    3

/* #define RBI_PARENT_INFO_COLOR_BIT     1 */
/* #define RBI_PARENT_INFO_DIRECTION_BIT 2 */

#define RBI_PARENT_INFO_DIRECTION_LEFT  0
#define RBI_PARENT_INFO_DIRECTION_RIGHT 2


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
	int parent_info;
};

struct RedBlackItorCursor {
	struct RedBlackItorNode *restrict *restrict stack;
	struct RedBlackItorNode *restrict path;
};

typedef void
(*RedBlackItorRestore)(struct RedBlackItorCursor *const restrict cursor,
		       struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_ITOR_TYPES_H_ */
