#ifndef RED_BLACK_ETOR_H_
#define RED_BLACK_ETOR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_link_offset.h" /* Node, LinkOffset, bool, size_t */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackEtor {
	const struct RedBlackNode *restrict *restrict cursor;
	const struct RedBlackLinkOffset *restrict offset;
	const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_asc_etor_init(struct RedBlackEtor *const restrict etor,
			const struct RedBlackNode *restrict root);

void
red_black_desc_etor_init(struct RedBlackEtor *const restrict etor,
			 const struct RedBlackNode *restrict root);

void
red_black_etor_reset(struct RedBlackEtor *const restrict etor,
		     const struct RedBlackNode *restrict root);

bool
red_black_etor_next(struct RedBlackEtor *const restrict etor,
			  void **const restrict key_ptr);

#endif /* ifndef RED_BLACK_ETOR_H_ */
