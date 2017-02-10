#ifndef RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_
#define RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <limits.h> /* INT_MAX */
#include <stdint.h> /* INT16|32_MAX */


#if (INT_MAX == INT16_MAX)
	/*    INT_MAX = 2¹⁵ - 1
	 * -> log₂(INT_MAX + 1) = 15 */
#	define _LOG_2_OF_INT_MAX_P1 15

#elif (INT_MAX == INT32_MAX)
	/*    INT_MAX = 2³¹ - 1
	 * -> log₂(INT_MAX + 1) = 31 */
#	define _LOG_2_OF_INT_MAX_P1 31

#else /* assume INT_MAX == INT64_MAX */
	/*    INT_MAX = 2⁶³ - 1
	 * -> log₂(INT_MAX + 1) = 63 */
#	define _LOG_2_OF_INT_MAX_P1 63
#endif /* if (INT_MAX == INT16_MAX) */

/* can handle trees of upto INT_MAX nodes (+1 for NULL terminator) */
#define RED_BLACK_STACK_COUNT ((2 * _LOG_2_OF_INT_MAX_P1) + 1)

#endif /* ifndef RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_ */
