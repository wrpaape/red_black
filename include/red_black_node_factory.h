#ifndef RED_BLACK_NODE_FACTORY_H_
#define RED_BLACK_NODE_FACTORY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_jump.h"	/* RedBlackJumpBuffer, RED_BLACK_JUMP_ERROR */
#include "red_black_node.h"	/* struct RedBlackNode */
#include <stddef.h>		/* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNodeFactoryBufferBlock {
	struct RedBlackNodeFactoryBufferBlock *next;
};

struct RedBlackNodeFactoryBuffer {
	char *restrict cursor;
	const char *restrict until;
	size_t expand;
	struct RedBlackNodeFactoryBufferBlock *restrict blocks;
};

typedef void
(*RedBlackNodeFactoryInitializer)(struct RedBlackNode *const restrict node,
				  const void *const key,
				  const bool is_red);

struct RedBlackNodeFactoryBlueprint {
	const RedBlackNodeFactoryInitializer initializer;
	const size_t size_node;
	const size_t init_expand;
};

struct RedBlackNodeFactory {
	struct RedBlackNode *restrict free;
	const struct RedBlackNodeFactoryBlueprint *restrict blueprint;
	struct RedBlackNodeFactoryBuffer buffer;
};


/* global variabless
 * ────────────────────────────────────────────────────────────────────────── */
extern const struct RedBlackNodeFactoryBlueprint node_factory_blueprint;
extern const struct RedBlackNodeFactoryBlueprint hnode_factory_blueprint;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
rbnf_init(struct RedBlackNodeFactory *const restrict factory,
	  const struct RedBlackNodeFactoryBlueprint *const restrict bp);

struct RedBlackNode *
rbnf_init_w_nodes(struct RedBlackNodeFactory *const restrict factory,
		  const struct RedBlackNodeFactoryBlueprint *const restrict bp,
		  const unsigned int count_nodes);

void
rbnf_reset(struct RedBlackNodeFactory *const restrict factory);

struct RedBlackNode *
rbnf_allocate(struct RedBlackNodeFactory *const restrict factory,
	      RedBlackJumpBuffer *const restrict jump_buffer);

struct RedBlackNode *
rbnf_allocate_nodes(struct RedBlackNodeFactory *const restrict factory,
		    const unsigned int count_nodes);

struct RedBlackNode *
rbnf_new(struct RedBlackNodeFactory *const restrict factory,
	 RedBlackJumpBuffer *const restrict jump_buffer,
	 const void *const key,
	 const bool is_red);

void
rbnf_free(struct RedBlackNodeFactory *const restrict factory,
	  struct RedBlackNode *const restrict node);

void
rbnf_destroy(struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_NODE_FACTORY_H_ */
