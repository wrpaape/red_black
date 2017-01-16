#include "red_black_node_factory.h" /* stddef included -> NULL */
#include "red_black_hnode.h"        /* RedBlackHNode, RedBlackHKey */
#include "red_black_malloc.h"	    /* RED_BLACK_MALLOC|FREE */

/* factory macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RBNF_INIT_EXPAND_COUNT	8


/* node initializers
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
rbnf_init_node(struct RedBlackNode *const restrict node,
	       const void *const key,
	       const bool is_red)
{
	node->key    = key;
	node->is_red = is_red;
	node->left   = NULL;
	node->right  = NULL;
}

static void
rbnf_init_hnode(struct RedBlackNode *const restrict node,
		const void *const key,
		const bool is_red)
{
	struct RedBlackHNode *restrict hnode;
	struct RedBlackHKey *restrict hkey;

	hnode = (struct RedBlackHNode *restrict) node;

	hkey = &hnode->hkey;

	rbnf_init_node(node,
		       (const void *) hkey,
		       is_red);

	/* copy input hash key to node */
	*hkey = *((const struct RedBlackHKey *restrict) key);
}


/* global variabless
 * ────────────────────────────────────────────────────────────────────────── */
const struct RedBlackNodeFactoryBlueprint node_factory_blueprint = {
	.size_node   = sizeof(struct RedBlackNode),
	.initializer = &rbnf_init_node,
	.init_expand = sizeof(struct RedBlackNode) * RBNF_INIT_EXPAND_COUNT
};

const struct RedBlackNodeFactoryBlueprint hnode_factory_blueprint = {
	.size_node   = sizeof(struct RedBlackHNode),
	.initializer = &rbnf_init_hnode,
	.init_expand = sizeof(struct RedBlackHNode) * RBNF_INIT_EXPAND_COUNT
};


static inline void
rbnfb_init(struct RedBlackNodeFactoryBuffer *const restrict buffer,
	   const size_t init_expand)
{
	buffer->cursor    = NULL;
	buffer->until     = NULL;
	buffer->expand    = init_expand;
	buffer->blocks    = NULL;
}

void
rbnf_init(struct RedBlackNodeFactory *const restrict factory,
	  const struct RedBlackNodeFactoryBlueprint *const restrict bp)
{
	factory->free      = NULL;
	factory->blueprint = bp;

	rbnfb_init(&factory->buffer,
		   bp->init_expand);
}

/* void * */
/* rbnf_init_wbuf(struct RedBlackNodeFactory *const restrict factory, */
/* 	       const struct RedBlackNodeFactoryBlueprint *const restrict bp, */
/* 	       const size_t buffer_size) */
/* { */
/* } */

void
rbnf_reset(struct RedBlackNodeFactory *const restrict factory)
{
	factory->buffer.expand = factory->blueprint->init_expand;
}


static inline struct RedBlackNode *
rbnfb_allocate(struct RedBlackNodeFactoryBuffer *const restrict buffer,
	       const size_t size_node,
	       RedBlackJumpBuffer *const restrict jump_buffer)
{
	char *restrict node;
	struct RedBlackNodeFactoryBufferBlock *restrict block;
	size_t allocate_size;

	node = buffer->cursor;

	if (node < buffer->until) {
		buffer->cursor += size_node;

	} else {
		/* current buffer completely filled, allocate new node buffer */
		allocate_size = sizeof(*block) + buffer->expand;

		buffer->expand *= 2;

		block = RED_BLACK_MALLOC(allocate_size);

		if (block == NULL)
			RED_BLACK_JUMP_3_ERROR(jump_buffer);

		/* push new buffer block into allocation list */
		block->next    = buffer->blocks;
		buffer->blocks = block;

		node = (char *) (block + 1);

		buffer->cursor = node + size_node;
		buffer->until  = ((char *) block) + allocate_size;
	}

	return (struct RedBlackNode *) node;
}

struct RedBlackNode *
rbnf_new(struct RedBlackNodeFactory *const restrict factory,
	 RedBlackJumpBuffer *const restrict jump_buffer,
	 const void *const key,
	 const bool is_red)
{
	struct RedBlackNode *restrict node;
	const struct RedBlackNodeFactoryBlueprint *restrict blueprint;
	RedBlackNodeFactoryInitializer initializer;

	node        = factory->free;
	blueprint   = factory->blueprint;
	initializer = blueprint->initializer;

	if (node == NULL)
		node = rbnfb_allocate(&factory->buffer,
				      blueprint->size_node,
				      jump_buffer);
	else
		factory->free = node->left;

	initializer(node,
		    key,
		    is_red);

	return node;
}


void
rbnf_free(struct RedBlackNodeFactory *const restrict factory,
	  struct RedBlackNode *const restrict node)
{
	node->left    = factory->free;
	factory->free = node;
}


void
rbnf_destroy(struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackNodeFactoryBufferBlock *restrict block;
	struct RedBlackNodeFactoryBufferBlock *restrict next;

	block = factory->buffer.blocks;

	while (block != NULL) {
		next = block->next;

		RED_BLACK_FREE(block);

		block = next;
	}
}
