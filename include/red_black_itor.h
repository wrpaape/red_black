#ifndef RED_BLACK_ITOR_H_
#define RED_BLACK_ITOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_types.h"   /* Node, Itor types, bool */
#include "red_black_node_factory.h" /* NodeFactory, size_t */
#include "red_black_stack_count.h"  /* RED_BLACK_STACK_COUNT */
#include "red_black_comparator.h"   /* Comparator (verify) */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackItorControlNode {
	size_t offset;
	RedBlackItorRestoreNode restore;
};

struct RedBlackItorController {
	struct RedBlackItorControlNode next;
	struct RedBlackItorControlNode prev;
};

struct RedBlackItor {
	struct RedBlackItorCursor cursor;
	const struct RedBlackItorController *restrict controller;
	struct RedBlackNodeFactory *restrict factory;
	struct RedBlackItorNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackItorNode path[RED_BLACK_STACK_COUNT];
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
		     struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory);

bool
red_black_itor_current(const struct RedBlackItor *const restrict itor,
		       void **const restrict key_ptr);

void
red_black_itor_drop(struct RedBlackItor *const restrict itor);

void
red_black_itor_skip(struct RedBlackItor *const restrict itor);

bool
red_black_itor_verify(const struct RedBlackItor *const restrict itor,
		      struct RedBlackNode *const restrict *restrict tree,
		      const RedBlackComparator comparator);

#endif /* ifndef RED_BLACK_ITOR_H_ */
