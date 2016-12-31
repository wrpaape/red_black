#ifndef RED_BLACK_INSERT_H_
#define RED_BLACK_INSERT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_allocator.h"  /* Node, Allocator, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_insert(struct RedBlackNode *restrict *const restrict root,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, tree NOT updated
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, tree updated
 *	0				OK, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_INSERT_H_ */k
