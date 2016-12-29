#ifndef RED_BLACK_PRINT_H_
#define RED_BLACK_PRINT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* RedBlackNode, bool */
#include <stddef.h>		/* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef size_t
(*RedBlackKeySizer)(const void *key);

typedef char *
(*RedBlackKeyPutter)(char *buffer,
		     const void *key);


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_print(const struct RedBlackNode *const restrict root,
		const RedBlackKeySizer key_sizer,
		const RedBlackKeyPutter key_putter);

/* RETURNS
 *	true	OK
 *	false	OUT OF MEMORY or write failure
 */

#endif /* ifndef RED_BLACK_PRINT_H_ */
