#include "red_black_allocator.h" /* stddef included -> NULL */
#include "red_black_hash_node.h" /* RedBlackHashNode, RedBlackHashKey */
#include "red_black_malloc.h"	 /* RED_BLACK_MALLOC|FREE */

/* allocator macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RBAB_INIT_EXPAND_COUNT	8


static inline void
rbab_init(struct RedBlackAllocatorBuffer *const restrict buffer,
	  const size_t size_node)
{
	buffer->cursor    = NULL;
	buffer->until     = NULL;
	buffer->size_node = size_node;
	buffer->expand    = size_node * RBAB_INIT_EXPAND_COUNT;
	buffer->blocks    = NULL;
}

void
rba_bucket_allocator_init(struct RedBlackAllocator *const restrict allocator)




void
rba_tree_allocator_init(struct RedBlackAllocator *const restrict allocator)
{
	allocator->free = NULL;

	rbab_init(&allocator->buffer);
}



static inline void *
rbab_allocate(struct RedBlackAllocatorBuffer *const restrict buffer,
	      const size_t size_node,
	      RedBlackJumpBuffer *const restrict jump_buffer)
{
	char *restrict node;
	struct RedBlackAllocatorBufferBlock *restrict block;
	size_t allocate_size;

	node = buffer->cursor;

	if (node < buffer->until) {
		buffer->cursor += buffer->size_node;

	} else {
		allocate_size = sizeof(struct RedBlackAllocatorBufferBlock)
			      + buffer->expand;

		buffer->expand *= 2;

		block = RED_BLACK_MALLOC(allocate_size);

		if (block == NULL)
			RED_BLACK_JUMP_3_ERROR(jump_buffer);

		block->next    = buffer->blocks;
		buffer->blocks = block;

		node = (char *) (block + 1);

		buffer->cursor = node + buffer->size_node;
		buffer->until  = ((char *) block) + allocate_size;
	}

	return (void *) node;
}

struct RedBlackNode *
rba_new(struct RedBlackAllocator *const restrict allocator,
	RedBlackJumpBuffer *const restrict jump_buffer,
	const void *const key,
	const bool is_red)
{
	struct RedBlackNode *restrict node;

	node = allocator->free;

	if (node == NULL)
		node = (struct RedBlackNode *) rbab_allocate(&allocator->buffer,
							     jump_buffer);
	else
		allocator->free = node->left;

	allocator->init(node,
			key,
			is_red);

	node->key    = key;
	node->is_red = is_red;
	node->left   = NULL;
	node->right  = NULL;

	return node;
}



void
rba_free(struct RedBlackAllocator *const restrict allocator,
	 struct RedBlackNode *const restrict node)
{
	node->left      = allocator->free;
	allocator->free = node;
}


void
rba_destroy(struct RedBlackAllocator *const restrict allocator)
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
