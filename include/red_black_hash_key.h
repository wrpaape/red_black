#ifndef RED_BLACK_HASH_NODE_H_
#define RED_BLACK_HASH_NODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdint.h>	/* uint64_t */
#include <stddef.h>	/* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHashKey {
	uint64_t hash;
	const void *key;
	size_t length;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hash_key_init(struct RedBlackHashKey *const restrict hash_key,
			const void *key,
			const size_t length);

int
red_black_hash_key_comparator(const void *key1,
			      const void *key2);

#endif /* ifndef RED_BLACK_HASH_NODE_H_ */
