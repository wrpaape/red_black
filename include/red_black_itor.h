#ifndef RED_BLACK_ITOR_H_
#define RED_BLACK_ITOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* Node, NodeFactory, bool, size_t */
#include "red_black_stack_count.h"  /* RED_BLACK_STACK_COUNT */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef struct RedBlackItorNode *restrict
(*RedBlackItorRestoreNode)(struct RedBlackItorNode *const restrict itor_node);

struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
	RedBlackItorRestoreNode restore;
};

struct RedBlackItorControlNode {
	size_t offset;
	RedBlackItorRestoreNode restore;
};

struct RedBlackItorController {
	struct RedBlackControlNode next;
	struct RedBlackControlNode prev;
};

struct RedBlackItor {
	struct RedBlackItorNode *restrict cursor;
	const struct RedBlackItorController *restrict control;
	struct RedBlackNodeFactory *restrict factory;
	struct RedBlackItorNode stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNodeFactory *const restrict factory);

void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNodeFactory *const restrict factory);

void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     struct RedBlackNode *restrict *const restrict tree);

bool
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr);

void
red_black_itor_drop(struct RedBlackItor *const restrict itor);

#endif /* ifndef RED_BLACK_ITOR_H_ */