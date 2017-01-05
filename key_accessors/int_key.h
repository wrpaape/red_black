#ifndef INT_KEY_H_
#define INT_KEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stddef.h>	/* size_t */
#include <stdbool.h>	/* bool */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
int_key_comparator(const void *key1,
		   const void *key2);

size_t
int_key_sizer(const void *key);

char *
int_key_putter(char *buffer,
	       const void *key);

#endif /* ifndef INT_KEY_H_ */
