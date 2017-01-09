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

static inline void
rba_init_node(struct RedBlackNode *const restrict node,
	      const void *const key,
	      const bool is_red)
{
	node->key    = key;
	node->is_red = is_red;
	node->left   = NULL;
	node->right  = NULL;
}

static void
rba_init_hash_node(struct RedBlackNode *const restrict node,
		   const void *const key,
		   const bool is_red)
{
	struct RedBlackHashNode *restrict hash_node;
	struct RedBlackHashKey *restrict hash_key;

	hash_node = (struct RedBlackHashNode *restrict) node;

	hash_key = &hash_node->hash_key;

	rba_init_node(node,
		      (const void *) hash_key,
		      is_red);

	*hash_key = *((const struct RedBlackHashKey *restrict) key);
}



static inline void
rba_allocator_init(struct RedBlackAllocator *const restrict allocator,
		   const size_t size_node,
		   const RedBlackAllocatorInitializer initializer)
{
	allocator->free = NULL;

	rbab_init(&allocator->buffer,
		  sizeof(struct RedBlackHashNode));

	allocator->initializer = initializer;
}

void
rba_tree_allocator_init(struct RedBlackAllocator *const restrict allocator)
{
	rba_allocator_init(allocator,
			   sizeof(struct RedBlackNode),
			   &rba_init_node);
}

void
rba_bucket_allocator_init(struct RedBlackAllocator *const restrict allocator)
{
	rba_allocator_init(allocator,
			   sizeof(struct RedBlackHashNode),
			   &rba_init_hash_node);
}


static inline struct RedBlackNode *
rbab_allocate(struct RedBlackAllocatorBuffer *const restrict buffer,
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

	return (struct RedBlackNode *) node;
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
		node = rbab_allocate(&allocator->buffer,
				     jump_buffer);
	else
		allocator->free = node->left;

	allocator->initializer(node,
			       key,
			       is_red);

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
