#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_TYPES_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_TYPES_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stddef.h> /* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef size_t
(*RedBlackKeySizer)(const void *key);

typedef char *
(*RedBlackKeyPutter)(char *buffer,
		     const void *key);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PRINT_TYPES_H_ */
