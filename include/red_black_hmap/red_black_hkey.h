#ifndef RED_BLACK_RED_BLACK_HKEY_H_
#define RED_BLACK_RED_BLACK_HKEY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdint.h> /* uint32|64_t, UINT32|64_MAX, uintptr_t, UINTPTR_MAX */
#include <stddef.h> /* size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
#if (UINTPTR_MAX < UINT64_MAX) /* toggle 32/64 bit hash */
	typedef uint32_t RedBlackHash;
#	define RED_BLACK_HASH_MAX UINT32_MAX

# else
	typedef uint64_t RedBlackHash;
#	define RED_BLACK_HASH_MAX UINT64_MAX
#endif

struct RedBlackHKey {
	RedBlackHash hash;
	const unsigned char *key;
	size_t length;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern const struct RedBlackHKey RED_BLACK_HKEY_MIN;
extern const struct RedBlackHKey RED_BLACK_HKEY_MAX;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hkey_init(struct RedBlackHKey *const restrict hkey,
		    const void *const key,
		    const size_t length);

int
red_black_hkey_comparator(const struct RedBlackHKey *const restrict hkey1,
			  const struct RedBlackHKey *const restrict hkey2);

#endif /* ifndef RED_BLACK_RED_BLACK_HKEY_H_ */
