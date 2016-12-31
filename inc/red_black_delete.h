#ifndef RED_BLACK_DELETE_H_
#define RED_BLACK_DELETE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_allocator.h"  /* Node, Allocator, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_delete(struct RedBlackNode *restrict *const restrict root,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const struct Key *const restrict key);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, tree NOT updated
 *
 * or return
 *
 *	true				OK, tree updated
 *	false				OK, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_DELETE_H_ */
