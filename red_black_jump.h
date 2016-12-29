#ifndef RED_BLACK_JUMP_H_
#define RED_BLACK_JUMP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <setjmp.h>


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef jmp_buf RedBlackJumpBuffer;


/* status macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_JUMP_VALUE_TRUE	4	/* ->  1 */
#define RED_BLACK_JUMP_VALUE_FALSE	2	/* ->  0 */
#define RED_BLACK_JUMP_VALUE_ERROR	1	/* -> -1 */

#define RED_BLACK_JUMP_STATUS(VALUE) (((VALUE) / 2) - 1)


/* jump macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_SET_JUMP(BUFFER)		setjmp(BUFFER)
#define RED_BLACK_LONG_JUMP(BUFFER_PTR, STATUS)	longjmp(*(BUFFER_PTR), STATUS)


#define RED_BLACK_JUMP_TRUE(BUFFER_PTR)					\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_TRUE)

#define RED_BLACK_JUMP_FALSE(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_FALSE)

#define RED_BLACK_JUMP_ERROR(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_ERROR)

#endif /* ifndef RED_BLACK_JUMP_H_ */
