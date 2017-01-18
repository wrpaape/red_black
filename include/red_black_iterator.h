#ifndef RED_BLACK_ITERATOR_H_
#define RED_BLACK_ITERATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"        /* RedBlackNode, bool */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef const struct RedBlackNode *restrict *restrict
(*RedBlackIteratorSeeker)(const struct RedBlackNode *restrict *restrict cursor,
			  const struct RedBlackNode *restrict node);

struct RedBlackIteratorBlueprint {
	const RedBlackIteratorSeeker updater;
	const RedBlackIteratorSeeker resetter;
};

struct RedBlackIterator {
	const struct RedBlackNode *restrict *restrict cursor;
	const struct RedBlackIteratorBlueprint *restrict blueprint;
	const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_asc_iterator_init(struct RedBlackIterator *const restrict iter,
			    const struct RedBlackNode *restrict root);

void
red_black_desc_iterator_init(struct RedBlackIterator *const restrict iter,
			     const struct RedBlackNode *restrict root);
void
red_black_iterator_reset(struct RedBlackIterator *const restrict iter,
			 const struct RedBlackNode *restrict root);

bool
red_black_iterator_next(struct RedBlackIterator *const restrict iter,
			void **const restrict key_ptr);

#endif /* ifndef RED_BLACK_ITERATOR_H_ */
