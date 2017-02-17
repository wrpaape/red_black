#ifndef RED_BLACK_RED_BLACK_COMMON_JUMP_H_
#define RED_BLACK_RED_BLACK_COMMON_JUMP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <setjmp.h> /* jmp_buf, setjmp, longjmp */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef jmp_buf RedBlackJumpBuffer;

/* must return 0 on 1st entry, nonzero on jump */
#define RED_BLACK_SET_JUMP(BUFFER)		setjmp(BUFFER)
#define RED_BLACK_LONG_JUMP(BUFFER, STATUS)	longjmp(BUFFER, STATUS)


/* status macros
 * map nonzero setjmp return values unconditionally to a domain including zero
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_JUMP_VALUE_3_TRUE	 2	/* ->  1 */
#define RED_BLACK_JUMP_VALUE_3_FALSE	 1	/* ->  0 */
#define RED_BLACK_JUMP_VALUE_3_ERROR	-2	/* -> -1 */

#define RED_BLACK_JUMP_3_STATUS(VALUE) ((VALUE) / 2)


#define RED_BLACK_JUMP_VALUE_2_TRUE	-1	/* ->  1  */
#define RED_BLACK_JUMP_VALUE_2_FALSE	 1	/* ->  0 */

#define RED_BLACK_JUMP_2_STATUS(VALUE) ((VALUE) < 0)


/* jump macros
 * ────────────────────────────────────────────────────────────────────────── */
/* jump w/ 3 possible return statuses */
#define RED_BLACK_JUMP_3_TRUE(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    RED_BLACK_JUMP_VALUE_3_TRUE)

#define RED_BLACK_JUMP_3_FALSE(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    RED_BLACK_JUMP_VALUE_3_FALSE)

#define RED_BLACK_JUMP_3_ERROR(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    RED_BLACK_JUMP_VALUE_3_ERROR)

/* jump w/ 2 possible return statuses */
#define RED_BLACK_JUMP_2_TRUE(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    RED_BLACK_JUMP_VALUE_2_TRUE)

#define RED_BLACK_JUMP_2_FALSE(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    RED_BLACK_JUMP_VALUE_2_FALSE)

/* jump w/ nonzero return status */
#define RED_BLACK_JUMP_1(BUFFER)					\
RED_BLACK_LONG_JUMP(BUFFER,						\
		    1)

#endif /* ifndef RED_BLACK_RED_BLACK_COMMON_JUMP_H_ */
