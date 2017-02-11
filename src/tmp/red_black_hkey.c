#include "red_black_hmap/red_black_hkey.h" /* RedBlackHKey, SIZE_MAX, NULL */
#include <limits.h>			   /* UINT_MAX, ULONG_MAX */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const struct RedBlackHKey RED_BLACK_HKEY_MIN = {
	.hash   = 0,
	.key    = NULL,
	.length = 0
};

const struct RedBlackHKey RED_BLACK_HKEY_MAX = {
	.hash   = RED_BLACK_HASH_MAX,
	.key    = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
	.length = SIZE_MAX
};


#if (RED_BLACK_HASH_MAX == UINT32_MAX)
/* taken from bob jenkin's website */
/* The mixing step */
#define mix32(a, b, c)							    \
{									    \
	a -= b;  a -= c;  a ^= (c >> 13); b -= c;  b -= a;  b ^= (a <<  8); \
	c -= a;  c -= b;  c ^= (b >> 13); a -= b;  a -= c;  a ^= (c >> 12); \
	b -= c;  b -= a;  b ^= (a << 16); c -= a;  c -= b;  c ^= (b >>  5); \
	a -= b;  a -= c;  a ^= (c >>  3); b -= c;  b -= a;  b ^= (a << 10); \
	c -= a;  c -= b;  c ^= (b >> 15);				    \
}

static inline RedBlackHash
rbhk_hash(register const unsigned char *k,  /* the key */
	  register const size_t length)     /* length of the key in bytes */
{
	register uint32_t a, b, c; /* the internal state */
	register size_t len;	   /* how many key bytes still need mixing */

	/* Set up the internal state */
	a = b = 0x9e3779b9; /* the golden ratio; an arbitrary value */
	c = 0xfedcba98;     /* variable initialization of internal state */

	/*---------------------------------------- handle most of the key */
	for (len = length; len >= 12; k += 12, len -= 12) {
		a += (   ((uint32_t) k[ 0])
		      + (((uint32_t) k[ 1]) <<  8)
		      + (((uint32_t) k[ 2]) << 16)
		      + (((uint32_t) k[ 3]) << 24));

		b += (   ((uint32_t) k[ 4])
		      + (((uint32_t) k[ 5]) <<  8)
		      + (((uint32_t) k[ 6]) << 16)
		      + (((uint32_t) k[ 7]) << 24));

		c += (   ((uint32_t) k[ 8])
		      + (((uint32_t) k[ 9]) <<  8)
		      + (((uint32_t) k[10]) << 16)
		      + (((uint32_t) k[11]) << 24));

		mix32(a, b, c);
	}

	/*------------------------------------- handle the last 11 bytes */
	c += length;

	switch (len) {
	/* all the case statements fall through */
	case 11: c += ((uint32_t) k[10] << 24);
	case 10: c += ((uint32_t) k[ 9] << 16);
	case  9: c += ((uint32_t) k[ 8] <<  8);
	/* the first byte of c is reserved for the length */
	case  8: b += ((uint32_t) k[ 7] << 24);
	case  7: b += ((uint32_t) k[ 6] << 16);
	case  6: b += ((uint32_t) k[ 5] <<  8);
	case  5: b += ((uint32_t) k[ 4]	     );
	case  4: a += ((uint32_t) k[ 3] << 24);
	case  3: a += ((uint32_t) k[ 2] << 16);
	case  2: a += ((uint32_t) k[ 1] <<  8);
	case  1: a += ((uint32_t) k[ 0]	     );
	/* case 0: nothing left  to add */
	}

	mix32(a, b, c);

	/*-------------------------------------------- report the result */
	return (RedBlackHash) c;
}
#undef mix32

#elif (RED_BLACK_HASH_MAX == UINT64_MAX)
/* taken from bob jenkin's website */
/* The mixing step */
#define mix64(a, b, c)							    \
{									    \
	a -= b;  a -= c;  a ^= (c >> 43); b -= c;  b -= a;  b ^= (a <<  9); \
	c -= a;  c -= b;  c ^= (b >>  8); a -= b;  a -= c;  a ^= (c >> 38); \
	b -= c;  b -= a;  b ^= (a << 23); c -= a;  c -= b;  c ^= (b >>  5); \
	a -= b;  a -= c;  a ^= (c >> 35); b -= c;  b -= a;  b ^= (a << 49); \
	c -= a;  c -= b;  c ^= (b >> 11); a -= b;  a -= c;  a ^= (c >> 12); \
	b -= c;  b -= a;  b ^= (a << 18); c -= a;  c -= b;  c ^= (b >> 22); \
}

static inline RedBlackHash
rbhk_hash(register const unsigned char *k, /* the key */
	  register const size_t length)    /* num bytes of key */
{
	register uint64_t a, b, c; /* the internal state */
	register size_t len;	   /* how many key bytes still need mixing */

	/* Set up the internal state */
	a = b = 0xfedcba9876543210ULL; /* initialization of internal state */
	c = 0x9e3779b97f4a7c13ULL;     /* the golden ratio; an arbitrary value */

	/*---------------------------------------- handle most of the key */
	for (len = length; len >= 24; k += 24, len -= 24) {
		a += (   ((uint64_t) k[ 0])
		      + (((uint64_t) k[ 1]) <<  8)
		      + (((uint64_t) k[ 2]) << 16)
		      + (((uint64_t) k[ 3]) << 24)
		      + (((uint64_t) k[ 4]) << 32)
		      + (((uint64_t) k[ 5]) << 40)
		      + (((uint64_t) k[ 6]) << 48)
		      + (((uint64_t) k[ 7]) << 56));

		b += (   ((uint64_t) k[ 8])
		      + (((uint64_t) k[ 9]) <<  8)
		      + (((uint64_t) k[10]) << 16)
		      + (((uint64_t) k[11]) << 24)
		      + (((uint64_t) k[12]) << 32)
		      + (((uint64_t) k[13]) << 40)
		      + (((uint64_t) k[14]) << 48)
		      + (((uint64_t) k[15]) << 56));

		c += (   ((uint64_t) k[16])
		      + (((uint64_t) k[17]) <<  8)
		      + (((uint64_t) k[18]) << 16)
		      + (((uint64_t) k[19]) << 24)
		      + (((uint64_t) k[20]) << 32)
		      + (((uint64_t) k[21]) << 40)
		      + (((uint64_t) k[22]) << 48)
		      + (((uint64_t) k[23]) << 56));

		mix64(a, b, c);
	}

	/*------------------------------------- handle the last 23 bytes */
	c += length;

	switch (len) {
	/* all the case statements fall through */
	case 23: c += (((uint64_t) k[22]) << 56);
	case 22: c += (((uint64_t) k[21]) << 48);
	case 21: c += (((uint64_t) k[20]) << 40);
	case 20: c += (((uint64_t) k[19]) << 32);
	case 19: c += (((uint64_t) k[18]) << 24);
	case 18: c += (((uint64_t) k[17]) << 16);
	case 17: c += (((uint64_t) k[16]) <<  8);
	/* the first byte of c is reserved for the length */
	case 16: b += (((uint64_t) k[15]) << 56);
	case 15: b += (((uint64_t) k[14]) << 48);
	case 14: b += (((uint64_t) k[13]) << 40);
	case 13: b += (((uint64_t) k[12]) << 32);
	case 12: b += (((uint64_t) k[11]) << 24);
	case 11: b += (((uint64_t) k[10]) << 16);
	case 10: b += (((uint64_t) k[ 9]) <<  8);
	case  9: b += (((uint64_t) k[ 8])      );
	case  8: a += (((uint64_t) k[ 7]) << 56);
	case  7: a += (((uint64_t) k[ 6]) << 48);
	case  6: a += (((uint64_t) k[ 5]) << 40);
	case  5: a += (((uint64_t) k[ 4]) << 32);
	case  4: a += (((uint64_t) k[ 3]) << 24);
	case  3: a += (((uint64_t) k[ 2]) << 16);
	case  2: a += (((uint64_t) k[ 1]) <<  8);
	case  1: a += (((uint64_t) k[ 0])      );
	/* case 0: nothing left to add */
	}

	mix64(a, b, c);

	/*-------------------------------------------- report the result */
	return (RedBlackHash) c;
}
#undef mix64
#endif /* if (RED_BLACK_HASH_MAX == UINT32_MAX) */


void
red_black_hkey_init(struct RedBlackHKey *const restrict hkey,
		    const void *const key,
		    const size_t length)
{
	hkey->hash   = rbhk_hash((const unsigned char *) key,
				 length);

	hkey->key    = (const unsigned char *) key;
	hkey->length = length;
}


static inline int
rbhk_memory_compare(const unsigned char *restrict key1,
		    const unsigned char *restrict key2,
		    const size_t length)
{
	int token1;
	int token2;
	unsigned long word1;
	unsigned long word2;
	const unsigned long *restrict word_key1;
	const unsigned long *restrict word_key2;

	const size_t count_words = length / sizeof(unsigned long);
	const size_t rem_bytes   = length % sizeof(unsigned long);

	/* compare misaligned bytes */
	switch (rem_bytes) {
#define COMPARE_BYTES()							\
	token1 = (int) *key1;						\
	token2 = (int) *key2;						\
	if (token1 != token2)						\
		return token1 - token2;					\
	++key1;								\
	++key2
#if (ULONG_MAX == UINT64_MAX)
	case 7: COMPARE_BYTES();
		/* fall through */
	case 6: COMPARE_BYTES();
		/* fall through */
	case 5: COMPARE_BYTES();
		/* fall through */
	case 4: COMPARE_BYTES();
		/* fall through */
#endif /* if (ULONG_MAX == UINT64_MAX) */
	case 3: COMPARE_BYTES();
		/* fall through */
	case 2: COMPARE_BYTES();
		/* fall through */
	case 1: COMPARE_BYTES();
		/* fall through */
#undef COMPARE_BYTES
	/* 0 byte offset from word boundary, do nothing */
	}

	/* compare aligned words */
	word_key1 = (const unsigned long *) key1;
	word_key2 = (const unsigned long *) key2;

	const unsigned long *const restrict word_key_until
	= word_key2 + count_words;

	while (word_key2 < word_key_until) {
		word1 = *word_key1;
		word2 = *word_key2;

		if (word1 != word2)
			return (word1 < word2) ? -1 : 1;

		++word_key1;
		++word_key2;
	}

	return 0; /* compared equal */
}

int
red_black_hkey_comparator(const struct RedBlackHKey *const restrict hkey1,
			  const struct RedBlackHKey *const restrict hkey2)
{
	const RedBlackHash hash1 = hkey1->hash;
	const RedBlackHash hash2 = hkey2->hash;

#if (RED_BLACK_HASH_MAX < UINT_MAX)
	if (hash1 != hash2)
		return (int) (hash1 - hash2);

#else
	if (hash1 < hash2)
		return -1;

	if (hash1 > hash2)
		return 1;
#endif /* if (RED_BLACK_HASH_MAX < UINT_MAX) */

	const unsigned char *const restrict key1 = hkey1->key;
	const unsigned char *const restrict key2 = hkey2->key;

	if (key1 == key2)
		return 0; /* ensure keys don't point to same memory */

	const size_t length1 = hkey1->length;
	const size_t length2 = hkey2->length;

#if (SIZE_MAX < UINT_MAX)
	if (length1 != length2)
		return (int) (length1 - length2);

#else
	if (length1 < length2)
		return -1;

	if (length1 > length2)
		return 1;
#endif /* if (RED_BLACK_HASH_MAX < UINT_MAX) */

	/* unsigned memory compare */
	return rbhk_memory_compare(key1,
				   key2,
				   length1);
}

