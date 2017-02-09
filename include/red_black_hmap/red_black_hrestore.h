#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HRESTORE_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HRESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */
#include "red_black_common/red_black_jump.h"	    /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* initial restore points */
void
red_black_hrestore_root(struct RedBlackHNode *restrict *const restrict tree,
			struct RedBlackHNode *const restrict root,
			struct RedBlackHNodeFactory *const restrict factory);

void
red_black_hrestore_node(struct RedBlackHNode *restrict *const restrict tree,
			struct RedBlackHNode *const restrict node,
			struct RedBlackHNodeFactory *const restrict factory,
			RedBlackJumpBuffer jump_buffer);

/* unwind restore points */
void
red_black_hrestore_l_bot(struct RedBlackHNode *restrict *const restrict tree,
			 struct RedBlackHNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);

void
red_black_hrestore_l_mid(struct RedBlackHNode *restrict *const restrict tree,
			 struct RedBlackHNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);

void
red_black_hrestore_r_bot(struct RedBlackHNode *restrict *const restrict tree,
			 struct RedBlackHNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);

void
red_black_hrestore_r_mid(struct RedBlackHNode *restrict *const restrict tree,
			 struct RedBlackHNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HRESTORE_H_ */
