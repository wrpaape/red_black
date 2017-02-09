#ifndef RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_
#define RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <limits.h> /* UINT_MAX */
#include <stdint.h> /* UINT16|32_MAX */


#if (UINT_MAX == UINT16_MAX)
#	define _LOG_2_UINT_MAX 16

#elif (UINT_MAX == UINT32_MAX)
#	define _LOG_2_UINT_MAX 32

#else /* assume UINT_MAX == UINT64_MAX */
#	define _LOG_2_UINT_MAX 64
#endif /* if (UINT_MAX == UINT16_MAX) */

/* can handle trees of upto UINT_MAX nodes (+1 for NULL terminator) */
#define RED_BLACK_STACK_COUNT ((2 * (_LOG_2_UINT_MAX + 1)) + 1)

#endif /* ifndef RED_BLACK_RED_BLACK_COMMON_STACK_COUNT_H_ */
