#ifndef RED_BLACK_INT_KEY_H_
#define RED_BLACK_INT_KEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stddef.h>	/* size_t */
#include <stdbool.h>	/* bool */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_int_key_init(void *restrict *const restrict key,
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
