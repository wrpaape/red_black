#include "red_black_tree/red_black_node_factory.h" /* stddef included -> NULL */
#include "red_black_common/red_black_malloc.h"	   /* RED_BLACK_MALLOC|FREE */

/* factory macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RBNF_INIT_EXPAND_COUNT	8
#define RBNF_INIT_EXPAND (sizeof(struct RedBlackHNode) * RBNF_INIT_EXPAND_COUNT)


static inline void
rbnfb_init(struct RedBlackHNodeFactoryBuffer *const restrict buffer,
	   struct RedBlackHNodeFactoryBufferBlock *const restrict first_block)

{
	buffer->cursor.current = NULL;
	buffer->cursor.until   = NULL;
	buffer->expand         = RBNF_INIT_EXPAND;
	buffer->blocks         = first_block;
}

static inline void
rbnf_do_init(struct RedBlackHNodeFactory *const restrict factory,
	     struct RedBlackHNodeFactoryBufferBlock *const restrict first_block)
{
	factory->free = NULL;

	rbnfb_init(&factory->buffer,
		   first_block);
}


void
rbnf_init(struct RedBlackHNodeFactory *const restrict factory)
{
	rbnf_do_init(factory,
		     NULL);
}


struct RedBlackHNode *
rbnf_init_w_nodes(struct RedBlackHNodeFactory *const restrict factory,
		  const unsigned int count_nodes)
{
	struct RedBlackHNodeFactoryBufferBlock *restrict first_block;
	size_t allocate_size;

	allocate_size = sizeof(*first_block)
		      + (sizeof(struct RedBlackHNode) * count_nodes);

	first_block = RED_BLACK_MALLOC(allocate_size);

	if (first_block == NULL)
		return NULL;

	rbnf_do_init(factory,
		     first_block);

	return (struct RedBlackHNode *) (first_block + 1);
}


static inline struct RedBlackHNode *
rbnfb_allocate(struct RedBlackHNodeFactoryBuffer *const restrict buffer,
	       RedBlackJumpBuffer jump_buffer)
{
	struct RedBlackHNode *restrict node;
	struct RedBlackHNodeFactoryBufferBlock *restrict block;
	size_t allocate_size;

	node = buffer->cursor.current;

	if (node < buffer->cursor.until) {
		/* node available in buffer, advance cursor */
		++(buffer->cursor.current);

	} else {
		/* current buffer exhausted, allocate new node buffer */
		allocate_size = sizeof(*block) + buffer->expand;

		buffer->expand *= 2;

		block = RED_BLACK_MALLOC(allocate_size);

		if (block == NULL)
			RED_BLACK_JUMP_3_ERROR(jump_buffer);

		/* push new buffer block into allocation list */
		block->next    = buffer->blocks;
		buffer->blocks = block;

		/* fetch first node */
		node = (struct RedBlackHNode *restrict) (block + 1);

		/* reset cursor */
		buffer->cursor.current = node + 1;
		buffer->cursor.until   = (const struct RedBlackHNode *restrict)
					 (((char *) block) + allocate_size);
	}

	return node;
}

struct RedBlackHNode *
rbnf_allocate(struct RedBlackHNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer)
{
	struct RedBlackHNode *restrict node;

	node = factory->free;

	if (node == NULL)
		node = rbnfb_allocate(&factory->buffer,
				      jump_buffer);
	else
		factory->free = node->left;

	return node;
}


static inline struct RedBlackHNode *
rbnfb_allocate_nodes(struct RedBlackHNodeFactoryBuffer *const restrict buffer,
		     const unsigned int count_nodes)
{
	struct RedBlackHNodeFactoryBufferBlock *restrict block;
	size_t allocate_size;

	allocate_size = sizeof(*block)
		      + (sizeof(struct RedBlackHNode) * count_nodes);

	block = RED_BLACK_MALLOC(allocate_size);

	if (block == NULL)
		return NULL;

	block->next    = buffer->blocks;
	buffer->blocks = block;

	return (struct RedBlackHNode *) (block + 1);
}


struct RedBlackHNode *
rbnf_allocate_nodes(struct RedBlackHNodeFactory *const restrict factory,
		    const unsigned int count_nodes)
{
	return rbnfb_allocate_nodes(&factory->buffer,
				    count_nodes);
}


struct RedBlackHNode *
rbnf_new(struct RedBlackHNodeFactory *const restrict factory,
	 RedBlackJumpBuffer jump_buffer,
	 const void *const key,
	 const bool is_red)
{
	struct RedBlackHNode *restrict node;

	node = rbnf_allocate(factory,
			     jump_buffer);

	/* initialize node */
	node->key    = key;
	node->is_red = is_red;
	node->left   = NULL;
	node->right  = NULL;

	return node;
}


void
rbnf_free(struct RedBlackHNodeFactory *const restrict factory,
	  struct RedBlackHNode *const restrict node)
{
	node->left    = factory->free;
	factory->free = node;
}


void
rbnf_destroy(struct RedBlackHNodeFactory *const restrict factory)
{
	struct RedBlackHNodeFactoryBufferBlock *restrict block;
	struct RedBlackHNodeFactoryBufferBlock *restrict next;

	block = factory->buffer.blocks;

	while (block != NULL) {
		next = block->next;

		RED_BLACK_FREE(block);

		block = next;
	}
}
