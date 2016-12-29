#ifndef RED_BLACK_ALLOCATOR_H_
#define RED_BLACK_ALLOCATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* struct RedBlackNode */
#include "red_black_jump.h"	/* RedBlackJumpBuffer, RED_BLACK_JUMP_ERROR */
#include <stdlib.h>		/* size_t, NULL */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackAllocatorNode {
	struct RedBlackNode node;
	struct RedBlackAllocatorNode *restrict *restrict prev_ptr;
	struct RedBlackAllocatorNode *restrict next;
};

struct RedBlackAllocatorBufferBlock {
	struct RedBlackAllocatorBufferBlock *next;
};

struct RedBlackAllocatorBuffer {
	struct RedBlackAllocatorNode *restrict current;
	const struct RedBlackAllocatorNode *restrict until;
	size_t expand;
	struct RedBlackAllocatorBufferBlock *restrict blocks;
};

struct RedBlackAllocator {
	struct RedBlackAllocatorNode *restrict free;
	struct RedBlackAllocatorBuffer buffer;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_allocator_init(struct RedBlackAllocator *const restrict allocator);

struct RedBlackNode *
red_black_allocator_allocate(struct RedBlackAllocator *const restrict allocator,
			     RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_allocator_free(struct RedBlackAllocator *const restrict allocator,
			 struct RedBlackNode *const restrict node);

void
red_black_allocator_destroy(struct RedBlackAllocator *const restrict allocator);

#endif /* ifndef RED_BLACK_ALLOCATOR_H_ */
