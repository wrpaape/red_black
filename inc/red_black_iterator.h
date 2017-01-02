#ifndef RED_BLACK_ITERATOR_H_
#define RED_BLACK_ITERATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"        /* RedBlackNode, bool */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef bool
RedBlackIteratorFetcher(struct RedBlackNode *restrict *const restrict current);


struct RedBlackIterator {
	const struct RedBlackNode *restrict *current;
	const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_iterator_init_pre(struct RedBlackIterator *const restrict iterator,
			    const struct RedBlackNode *const restrict);

#endif /* ifndef RED_BLACK_ITERATOR_H_ */
