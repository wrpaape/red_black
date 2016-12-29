#ifndef RED_BLACK_MUTATOR_H_
#define RED_BLACK_MUTATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"
#include "red_black_allocator.h"


struct RedBlackMutator {
	const RedBlackComparator comparator;
	const void *const key;
	struct RedBlackAllocator *const restrict allocator;
	RedBlackJumpBuffer *const restrict jump_buffer;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_MUTATOR_COMPARE(MUTATOR,				\
				  KEY)					\
(MUTATOR)->comparator((MUTATOR)->key,					\
		      KEY)

#define RED_BLACK_MUTATOR_ALLOCATE(MUTATOR)				\
red_black_allocator_allocate((MUTATOR)->allocator,			\
			     (MUTATOR)->jump_buffer)

#define RED_BLACK_MUTATOR_FREE(MUTATOR,					\
			       NODE)					\
red_black_allocator_free((MUTATOR)->allocator,				\
			 NODE)


#endif /* ifndef RED_BLACK_MUTATOR_H_ */
