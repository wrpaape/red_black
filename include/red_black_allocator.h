#ifndef RED_BLACK_ALLOCATOR_H_
#define RED_BLACK_ALLOCATOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* struct RedBlackNode */
#include "red_black_jump.h"	/* RedBlackJumpBuffer, RED_BLACK_JUMP_ERROR */
#include <stddef.h>		/* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackAllocatorBufferBlock {
	struct RedBlackAllocatorBufferBlock *next;
};

struct RedBlackAllocatorBuffer {
	char *restrict cursor;
	const char *restrict until;
	size_t size_node;
	size_t expand;
	struct RedBlackAllocatorBufferBlock *restrict blocks;
};

typedef void
(*RedBlackAllocatorInitializer)(struct RedBlackNode *const restrict node,
				const void *const key,
				const bool is_red);

struct RedBlackAllocator {
	struct RedBlackNode *restrict free;
	struct RedBlackAllocatorBuffer buffer;
	RedBlackAllocatorInitializer init;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
rba_bucket_allocator_init(struct RedBlackAllocator *const restrict allocator);

void
rba_tree_allocator_init(struct RedBlackAllocator *const restrict allocator);

struct RedBlackNode *
rba_new(struct RedBlackAllocator *const restrict allocator,
	RedBlackJumpBuffer *const restrict jump_buffer,
	const void *const key,
	const bool is_red);

void
rba_free(struct RedBlackAllocator *const restrict allocator,
	 struct RedBlackNode *const restrict node);

void
rba_destroy(struct RedBlackAllocator *const restrict allocator);

#endif /* ifndef RED_BLACK_ALLOCATOR_H_ */
