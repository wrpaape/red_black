#ifndef RED_BLACK_JUMP_H_
#define RED_BLACK_JUMP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <setjmp.h> /* jmp_buf, setjmp, longjmp */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef jmp_buf RedBlackJumpBuffer;


/* status macros
 * map nonzero setjmp return values unconditionally to a domain including zero
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_JUMP_VALUE_3_TRUE	4	/* ->  1 */
#define RED_BLACK_JUMP_VALUE_3_FALSE	2	/* ->  0 */
#define RED_BLACK_JUMP_VALUE_3_ERROR	1	/* -> -1 */

#define RED_BLACK_JUMP_3_STATUS(VALUE) (((VALUE) / 2) - 1)


#define RED_BLACK_JUMP_VALUE_2_TRUE	2	/* ->  1  */
#define RED_BLACK_JUMP_VALUE_2_FALSE	1	/* ->  0 */

#define RED_BLACK_JUMP_2_STATUS(VALUE) (VALUE - 1)


/* jump macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_SET_JUMP(BUFFER)		setjmp(BUFFER)
#define RED_BLACK_LONG_JUMP(BUFFER_PTR, STATUS)	longjmp(*(BUFFER_PTR), STATUS)


/* jump w/ 3 possible return statuses */
#define RED_BLACK_JUMP_3_TRUE(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_3_TRUE)

#define RED_BLACK_JUMP_3_FALSE(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_3_FALSE)

#define RED_BLACK_JUMP_3_ERROR(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_3_ERROR)


/* jump w/ 2 possible return statuses */
#define RED_BLACK_JUMP_2_TRUE(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_2_TRUE)

#define RED_BLACK_JUMP_2_FALSE(BUFFER_PTR)				\
RED_BLACK_LONG_JUMP(BUFFER_PTR,						\
		    RED_BLACK_JUMP_VALUE_2_FALSE)

#endif /* ifndef RED_BLACK_JUMP_H_ */
