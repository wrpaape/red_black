#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_NODE_FACTORY_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_NODE_FACTORY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_common/red_black_jump.h" /* JumpBuffer, JUMP_ERROR */
#include "red_black_tree/red_black_node.h"   /* struct RedBlackNode */
#include <stddef.h>			     /* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNodeFactoryBufferCursor {
	struct RedBlackNode *restrict current;
	const struct RedBlackNode *restrict until;
};

struct RedBlackNodeFactoryBufferBlock {
	struct RedBlackNodeFactoryBufferBlock *next;
};

struct RedBlackNodeFactoryBuffer {
	struct RedBlackNodeFactoryBufferCursor cursor;
	size_t expand;
	struct RedBlackNodeFactoryBufferBlock *restrict blocks;
};

struct RedBlackNodeFactory {
	struct RedBlackNode *restrict free;
	struct RedBlackNodeFactoryBuffer buffer;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
rbnf_init(struct RedBlackNodeFactory *const restrict factory);

struct RedBlackNode *
rbnf_init_w_nodes(struct RedBlackNodeFactory *const restrict factory,
		  const unsigned int count_nodes);

struct RedBlackNode *
rbnf_allocate(struct RedBlackNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer);

struct RedBlackNode *
rbnf_allocate_nodes(struct RedBlackNodeFactory *const restrict factory,
		    const unsigned int count_nodes);

struct RedBlackNode *
rbnf_new(struct RedBlackNodeFactory *const restrict factory,
	 RedBlackJumpBuffer jump_buffer,
	 const void *const key,
	 const bool is_red);

void
rbnf_free(struct RedBlackNodeFactory *const restrict factory,
	  struct RedBlackNode *const restrict node);

void
rbnf_destroy(struct RedBlackNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_NODE_FACTORY_H_ */
