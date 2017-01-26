#ifndef RED_BLACK_ITOR_H_
#define RED_BLACK_ITOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"        /* RedBlackNode, bool */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		   /* size_t, offsetof() */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
};

struct RedBlackItor {
	const struct RedBlackNode *restrict *restrict cursor;
	const struct RedBlackItorNode *restrict stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			const struct RedBlackNode *restrict root);

void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 const struct RedBlackNode *restrict root);

void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     const struct RedBlackNode *restrict root);

bool
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr);

void
red_black_itor_drop(struct RedBlackItor *const restrict itor);

#endif /* ifndef RED_BLACK_ITOR_H_ */
