#ifndef RED_BLACK_ITOR_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_itor_types.h"   /* Node, NodeFactory ItorNode, ItorCursor */
#include "red_black_stack_count.h"  /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		    /* NULL */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
/* restore red-black properties ONLY */
typedef void
(*RedBlackItorRestoreValid)(struct RedBlackItorNode *const restrict itor_node);

/* restore cursor ONLY */
typedef void
(*RedBlackItorRestoreCursor)(struct RedBlackItorCursor *const restrict cursor,
			     struct RedBlackItorNode *const restrict itor_node);

/* restore red-black properties AND cursor */
typedef void
(*RedBlackItorRestoreBoth)(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackItorNode *const restrict itor_node);

#define RED_BLACK_ITOR_RESTORE_H_
#endif /* ifndef RED_BLACK_ITOR_RESTORE_H_ */
