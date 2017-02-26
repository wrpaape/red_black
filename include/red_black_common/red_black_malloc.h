#ifndef RED_BLACK_RED_BLACK_COMMON_MALLOC_H_
#define RED_BLACK_RED_BLACK_COMMON_MALLOC_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h> /* malloc, realloc free */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
#if 1
/* must be thread-safe, return NULL on allocation failure */
#define RED_BLACK_MALLOC(SIZE)		malloc(SIZE)
#define RED_BLACK_CALLOC(COUNT, SIZE)	calloc(COUNT, SIZE)
#define RED_BLACK_REALLOC(PTR, SIZE)	realloc(PTR, SIZE)
#define RED_BLACK_FREE(PTR)		free(PTR)
#else
#include <stdio.h>
#define RED_BLACK_MALLOC(SIZE)		(puts("malloc"), malloc(SIZE))
#define RED_BLACK_CALLOC(COUNT, SIZE)	(puts("calloc"), calloc(COUNT, SIZE))
#define RED_BLACK_REALLOC(PTR, SIZE)	(puts("realloc"), realloc(PTR, SIZE))
#define RED_BLACK_FREE(PTR)		(puts("free"), free(PTR))
#endif


#endif /* ifndef RED_BLACK_RED_BLACK_COMMON_MALLOC_H_ */
