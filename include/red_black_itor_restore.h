#ifndef RED_BLACK_ITOR_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_cursor.h"           /* Node, ItorNode, ItorCursor */
#include "red_black_itor_node_parent_info.h" /* RBNI_PARENT_INFO macros */
#include "red_black_stack_count.h"	     /* RED_BLACK_STACK_COUNT */
#include <stddef.h>			     /* NULL */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
/* restore cursor ONLY or red-black properties AND cursor */
typedef void
(*RedBlackItorRestoreNode)(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_node);


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* restore red-black properties ONLY */
void
rbi_restore_valid_b(struct RedBlackItorNode *const restrict itor_node);

void
rbi_restore_valid_r(struct RedBlackItorNode *const restrict itor_node);

#define RED_BLACK_ITOR_RESTORE_H_
#endif /* ifndef RED_BLACK_ITOR_RESTORE_H_ */
