#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	  /* RedBlackNode, bool */
#include "red_black_tree/red_black_print_types.h" /* KeySizer|Putter, size_t */

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

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_H_ */
