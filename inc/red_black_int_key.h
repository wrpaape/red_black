#ifndef RED_BLACK_INT_KEY_H_
#define RED_BLACK_INT_KEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdint.h>	/* intptr_t */
#include <stddef.h>	/* size_t */
#include <bool.h>	/* bool */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef intptr_t RedBlackIntKey;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_int_key_make(RedBlackIntKey *const restrict key,
		       const char *const restrict string);


/* accessors */
int
red_black_int_key_comparator(const void *key1,
			     const void *key2);

size_t
red_black_int_key_sizer(const void *key);

char *
red_black_int_key_putter(char *buffer,
			 const void *key);

#endif /* ifndef RED_BLACK_INT_KEY_H_ */
