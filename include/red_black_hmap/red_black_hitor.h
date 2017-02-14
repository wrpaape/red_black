#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HITOR_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HITOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h"	    /* RedBlackHNode */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */
#include <stddef.h>				    /* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHItor {
	const struct RedBlackHNode *restrict *restrict cursor;
	const struct RedBlackHNode *restrict stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hitor_init(struct RedBlackHItor *const restrict itor,
		     const struct RedBlackHNode *restrict root);

void
red_black_hitor_reset(struct RedBlackHItor *const restrict itor,
		      const struct RedBlackHNode *restrict root);

bool
red_black_hitor_next(struct RedBlackHItor *const restrict itor,
		     void **const restrict key_ptr,
		     size_t *const restrict length_ptr);

const struct Hkey *restrict
red_black_hitor_next_hkey(struct RedBlackHItor *const restrict itor);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HITOR_H_ */
