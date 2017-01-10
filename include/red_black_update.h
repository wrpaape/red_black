#ifndef RED_BLACK_UPDATE_H_
#define RED_BLACK_UPDATE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_allocator.h"  /* Node, Allocator, and JumpBuffer */

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_update(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackAllocator *const restrict allocator,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key,
		 void **const restrict old_ptr);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, old_ptr unset
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, successful key swap, old_ptr is set
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion, old_ptr unset
 *	0				OK, successful key swap, old_ptr is set
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_UPDATE_H_ */
