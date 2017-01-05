#ifndef STR_KEY_H_
#define STR_KEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stddef.h>	/* size_t */
#include <stdbool.h>	/* bool */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
str_key_comparator(const void *key1,
		   const void *key2);

size_t
str_key_sizer(const void *key);

char *
str_key_putter(char *buffer,
	       const void *key);

#endif /* ifndef STR_KEY_H_ */

