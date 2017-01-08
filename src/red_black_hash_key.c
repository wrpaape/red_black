#include "red_black_hash_key.h"	/* RedBlackHashKey */

#define JHASH_INIT_LEVEL 0xfedcba987654321

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

static inline uint64_t
jhash64(register const unsigned char *k, /* the key */
	register const size_t length,    /* num bytes of key */
	register const uint64_t level)   /* prev hash or arb val */
{
	register uint64_t a, b, c; /* the internal state */
	register size_t len;	   /* how many key bytes still need mixing */

	/* Set up the internal state */
	a = b = level;		  /* variable initialization of internal state */
	c = 0x9e3779b97f4a7c13LL; /* the golden ratio; an arbitrary value */

	/*---------------------------------------- handle most of the key */
	for (len = length; len >= 24; k += 24, len -= 24) {

		a += (  k[ 0]			   + (((uint64_t) k[ 1]) <<  8)
		      + (((uint64_t) k[ 2]) << 16) + (((uint64_t) k[ 3]) << 24)
		      + (((uint64_t) k[ 4]) << 32) + (((uint64_t) k[ 5]) << 40)
		      + (((uint64_t) k[ 6]) << 48) + (((uint64_t) k[ 7]) << 56));

		b += (  k[ 8]			   + (((uint64_t) k[ 9]) <<  8)
		      + (((uint64_t) k[10]) << 16) + (((uint64_t) k[11]) << 24)
		      + (((uint64_t) k[12]) << 32) + (((uint64_t) k[13]) << 40)
		      + (((uint64_t) k[14]) << 48) + (((uint64_t) k[15]) << 56));

		c += (  k[16]			   + (((uint64_t) k[17]) <<  8)
		      + (((uint64_t) k[18]) << 16) + (((uint64_t) k[19]) << 24)
		      + (((uint64_t) k[20]) << 32) + (((uint64_t) k[21]) << 40)
		      + (((uint64_t) k[22]) << 48) + (((uint64_t) k[23]) << 56));

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
	return c;
}
#undef mix64


void
red_black_hash_key_init(struct RedBlackHashKey *const restrict hash_key,
			const void *key,
			const size_t length)
{
	hash_key->hash = jhash64((const unsigned char *) key,
				 length,
				 0xfedcba98);
}

