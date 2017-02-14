#include "red_black_hmap/red_black_hitor.h" /* HItor, HNode, NULL */


static inline const struct RedBlackHNode *restrict *restrict
rbhi_update(const struct RedBlackHNode *restrict *restrict cursor,
	    const struct RedBlackHNode *restrict node)
{
	node = node->right;

	/* if can't go forward, unwind stack one level */
	if (node == NULL)
		return cursor - 1;

	/* find next successor, overwrite cursor, keep track of stack */
	while (1) {
		*cursor = node;

		node = node->left;

		if (node == NULL)
			return cursor; /* return pointer to successor */

		++cursor;
	}
}


static inline const struct RedBlackHNode *restrict *restrict
rbhi_reset(const struct RedBlackHNode *restrict *restrict cursor,
	   const struct RedBlackHNode *restrict node)
{
	/* set cursor left-most node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = node->left;
	}

	return cursor;
}


void
red_black_hitor_init(struct RedBlackHItor *const restrict itor,
		     const struct RedBlackHNode *restrict root)
{
	const struct RedBlackHNode *restrict *restrict cursor;

	cursor = &itor->stack[0];

	*cursor = NULL; /* mark top of stack */

	itor->cursor = rbhi_reset(cursor,
				  root);
}


void
red_black_hitor_reset(struct RedBlackHItor *const restrict itor,
		      const struct RedBlackHNode *restrict root)
{
	itor->cursor = rbhi_reset(&itor->stack[0],
				  root);
}


bool
red_black_hitor_next(struct RedBlackHItor *const restrict itor,
		     void **const restrict key_ptr,
		     size_t *const restrict length_ptr)
{
	const struct RedBlackHNode *restrict *restrict cursor;
	const struct RedBlackHNode *restrict node;
	bool has_next;

	cursor   = itor->cursor;
	node     = *cursor;
	has_next = (node != NULL);

	if (has_next) {
		itor->cursor = rbhi_update(cursor,
					   node);

		*key_ptr    = (void *) node->hkey.key;
		*length_ptr = node->hkey.length;
	}

	return has_next;
}


/* internal use, will only be called if more keys available */
const struct RedBlackHKey *restrict
red_black_hitor_next_hkey(struct RedBlackHItor *const restrict itor)
{
	const struct RedBlackHNode *restrict *restrict cursor;
	const struct RedBlackHNode *restrict node;

	cursor = itor->cursor;
	node   = *cursor;

	if (node == NULL)
		return NULL;

	itor->cursor = rbhi_update(cursor,
				   node);

	return &node->hkey;
}
