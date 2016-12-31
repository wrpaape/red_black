#include "red_black_allocator.h" /* stddef included -> NULL */
#include "red_black_malloc.h"	 /* RED_BLACK_MALLOC|FREE */


/* allocator macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_ALLOCATOR_BUFFER_INIT_EXPAND_COUNT	8
#define RED_BLACK_ALLOCATOR_BUFFER_INIT_EXPAND_SIZE			\
(  sizeof(struct RedBlackAllocatorNode)					\
 * RED_BLACK_ALLOCATOR_BUFFER_INIT_EXPAND_COUNT)


static inline void
rba_buffer_init(struct RedBlackAllocatorBuffer *const restrict buffer)
{
	buffer->current = NULL;
	buffer->until   = NULL;
	buffer->expand  = RED_BLACK_ALLOCATOR_BUFFER_INIT_EXPAND_SIZE;
	buffer->blocks  = NULL;
}

void
red_black_allocator_init(struct RedBlackAllocator *const restrict allocator)
{
	allocator->free = NULL;

	rba_buffer_init(&allocator->buffer);
}



static inline struct RedBlackAllocatorNode *
rba_buffer_allocate(struct RedBlackAllocatorBuffer *const restrict buffer,
		    RedBlackJumpBuffer *const restrict jump_buffer)
{
	struct RedBlackAllocatorNode *restrict allocator_node;
	struct RedBlackAllocatorBufferBlock *restrict block;
	size_t allocate_size;

	allocator_node = buffer->current;

	if (allocator_node < buffer->until) {
		++(buffer->current);

	} else {
		allocate_size = sizeof(struct RedBlackAllocatorBufferBlock)
			      + buffer->expand;

		buffer->expand *= 2;

		block = RED_BLACK_MALLOC(allocate_size);

		if (block == NULL)
			RED_BLACK_JUMP_3_ERROR(jump_buffer);

		block->next    = buffer->blocks;
		buffer->blocks = block;

		allocator_node = (struct RedBlackAllocatorNode *) (block + 1);

		buffer->current = allocator_node + 1;
		buffer->until   = (struct RedBlackAllocatorNode *)
				  (((char *) block) + allocate_size);
	}

	return allocator_node;
}

struct RedBlackNode *
red_black_allocator_allocate(struct RedBlackAllocator *const restrict allocator,
			     RedBlackJumpBuffer *const restrict jump_buffer)
{
	struct RedBlackAllocatorNode *restrict allocator_node;

	allocator_node = allocator->free;

	if (allocator_node == NULL)
		allocator_node = rba_buffer_allocate(&allocator->buffer,
						     jump_buffer);
	else
		allocator->free = allocator_node->next;

	return (struct RedBlackNode *) allocator_node;
}



void
red_black_allocator_free(struct RedBlackAllocator *const restrict allocator,
			 struct RedBlackNode *const restrict node)
{
	struct RedBlackAllocatorNode *restrict allocator_node;

	allocator_node = (struct RedBlackAllocatorNode *restrict) node;

	allocator_node->next = allocator->free;
	allocator->free	     = allocator_node;
}



void
red_black_allocator_destroy(struct RedBlackAllocator *const restrict allocator)
{
	struct RedBlackAllocatorBufferBlock *restrict block;
	struct RedBlackAllocatorBufferBlock *restrict next;

	block = allocator->buffer.blocks;

	while (block != NULL) {
		next = block->next;

		RED_BLACK_FREE(block);

		block = next;
	}
}
