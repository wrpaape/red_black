#ifndef RED_BLACK_ITERATOR_H_
#define RED_BLACK_ITERATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"        /* RedBlackNode, bool */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef const struct RedBlackNode *restrict *restrict
(*RedBlackIteratorUpdater)(const struct RedBlackNode *restrict *restrict cursor,
			   const struct RedBlackNode *restrict node);

struct _RedBlackIterator {
	const struct RedBlackNode *restrict *restrict cursor;
	RedBlackIteratorUpdater update;
	const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};

typedef struct _RedBlackIterator RedBlackIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_iterator_init_asc(RedBlackIterator *const restrict iterator,
			    const struct RedBlackNode *restrict node);

void
red_black_iterator_init_desc(RedBlackIterator *const restrict iterator,
			     const struct RedBlackNode *restrict node);

bool
red_black_iterator_next(RedBlackIterator *const restrict iterator,
			const void **const restrict key_ptr);

#endif /* ifndef RED_BLACK_ITERATOR_H_ */
