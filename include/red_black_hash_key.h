#ifndef RED_BLACK_HASH_KEY_H_
#define RED_BLACK_HASH_KEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdint.h>	/* uint32|64_t, UINT32|64_MAX */
#include <stddef.h>	/* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
#if 0 /* toggle 32/64 bit hash */
	typedef uint32_t RedBlackHash;
#	define RED_BLACK_HASH_MAX UINT32_MAX

# else
	typedef uint64_t RedBlackHash;
#	define RED_BLACK_HASH_MAX UINT64_MAX
#endif

struct RedBlackHashKey {
	RedBlackHash hash;
	const unsigned char *key;
	size_t length;
};


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hash_key_init(struct RedBlackHashKey *const restrict hash_key,
			const void *const key,
			const size_t length);

int
red_black_hash_key_comparator(const void *key1,
			      const void *key2);

#endif /* ifndef RED_BLACK_HASH_KEY_H_ */