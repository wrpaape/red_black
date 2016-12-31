#ifndef RED_BLACK_MALLOC_H_
#define RED_BLACK_MALLOC_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h> /* malloc, free */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_MALLOC(SIZE)		malloc(SIZE)
#define RED_BLACK_FREE(ALLOCATION)	free(ALLOCATION)

#endif /* ifndef RED_BLACK_MALLOC_H_ */
