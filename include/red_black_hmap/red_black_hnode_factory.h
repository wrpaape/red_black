#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_FACTORY_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_FACTORY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_common/red_black_jump.h" /* JumpBuffer, JUMP_ERROR */
#include "red_black_hmap/red_black_hnode.h"  /* struct RedBlackHNode */
#include <stddef.h>			     /* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHNodeFactoryBufferCursor {
	struct RedBlackHNode *restrict current;
	const struct RedBlackHNode *restrict until;
};

struct RedBlackHNodeFactoryBufferBlock {
	struct RedBlackHNodeFactoryBufferBlock *next;
};

struct RedBlackHNodeFactoryBuffer {
	struct RedBlackHNodeFactoryBufferCursor cursor;
	size_t expand;
	struct RedBlackHNodeFactoryBufferBlock *restrict blocks;
};

struct RedBlackHNodeFactory {
	struct RedBlackHNode *restrict free;
	struct RedBlackHNodeFactoryBuffer buffer;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
rbhnf_init(struct RedBlackHNodeFactory *const restrict factory);

struct RedBlackHNode *
rbhnf_init_w_nodes(struct RedBlackHNodeFactory *const restrict factory,
		   const unsigned int count_nodes);

struct RedBlackHNode *
rbhnfb_allocate(struct RedBlackHNodeFactoryBuffer *const restrict buffer,
		RedBlackJumpBuffer jump_buffer);

struct RedBlackHNode *
rbhnf_allocate(struct RedBlackHNodeFactory *const restrict factory,
	       RedBlackJumpBuffer jump_buffer);

struct RedBlackHNode *
rbhnf_allocate_nodes(struct RedBlackHNodeFactory *const restrict factory,
		     const unsigned int count_nodes);

struct RedBlackHNode *
rbhnf_new(struct RedBlackHNodeFactory *const restrict factory,
	  RedBlackJumpBuffer jump_buffer,
	  const struct RedBlackHKey *const restrict hkey,
	  const bool is_red);

void
rbhnf_free(struct RedBlackHNodeFactory *const restrict factory,
	   struct RedBlackHNode *const restrict node);

void
rbhnf_reset(struct RedBlackHNodeFactory *const restrict factory);

void
rbhnf_destroy(struct RedBlackHNodeFactory *const restrict factory);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_FACTORY_H_ */
