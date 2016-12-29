#include "key_utils.h"

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const struct Key KEY_MIN = {
#if USE_HASH
	.hash   = HASH_MIN,
#endif /* if USE_HASH */
	.string = STRING_MIN
};

const struct Key KEY_MAX = {
#if USE_HASH
	.hash   = HASH_MAX,
#endif /* if USE_HASH */
	.string = STRING_MAX
};

void
exit_on_failure(const char *const restrict failure,
		const size_t length_failure)
{
	(void) write(STDERR_FILENO,
		     failure,
		     length_failure);
	exit(EXIT_FAILURE);
	__builtin_unreachable();
}

void
exit_on_success(const char *const restrict message,
		const size_t length_message)
{
	const int status = (write(STDOUT_FILENO,
				  message,
				  length_message) == (ssize_t) length_message);

	if (!status)
		(void) write(STDERR_FILENO,
			     "exit_on_success failure: write\n",
			     sizeof("exit_on_success failure: write\n") - 1);

	exit(status);
	__builtin_unreachable();
}

static inline const unsigned char *
store_string(const unsigned char *restrict string,
	     const size_t length)
{
	static unsigned char buffer[2048];
	static size_t rem_size		     = sizeof(buffer);
	static unsigned char *restrict alloc = &buffer[0];

	if (length < rem_size) {
		(void) memcpy(alloc,
			      string,
			      length);

		alloc[length] = '\0';

		const unsigned char *const restrict store = alloc;
		const size_t size = length + 1;

		alloc    += size;
		rem_size -= size;
		return store;
	}

	EXIT_ON_FAILURE("store_string failure: out of space");
	__builtin_unreachable();
}


#if USE_HASH
#if 0
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
#endif

/* The mixing step */
#define mix32(a, b, c)							    \
{									    \
	a -= b;  a -= c;  a ^= (c >> 13); b -= c;  b -= a;  b ^= (a <<  8); \
	c -= a;  c -= b;  c ^= (b >> 13); a -= b;  a -= c;  a ^= (c >> 12); \
	b -= c;  b -= a;  b ^= (a << 16); c -= a;  c -= b;  c ^= (b >>  5); \
	a -= b;  a -= c;  a ^= (c >>  3); b -= c;  b -= a;  b ^= (a << 10); \
	c -= a;  c -= b;  c ^= (b >> 15);				    \
}

static inline uint32_t
jhash32(register const unsigned char *k,  /* the key */
	register const size_t length,     /* length of the key in bytes */
	register const uint32_t init_val) /* prev hash or arb val */
{
	register uint32_t a, b, c; /* the internal state */
	register size_t len;	   /* how many key bytes still need mixing */

	/* Set up the internal state */
	a = b = 0x9e3779b9; /* the golden ratio; an arbitrary value */
	c = init_val;       /* variable initialization of internal state */

	/*---------------------------------------- handle most of the key */
	for (len = length; len >= 12; k += 12, len -= 12) {

		a += (  k[0]			   + (((uint32_t) k[ 1]) <<  8)
		      + (((uint32_t) k[ 2]) << 16) + (((uint32_t) k[ 3]) << 24));

		b += (  k[4]			   + (((uint32_t) k[ 5]) <<  8)
		      + (((uint32_t) k[ 6]) << 16) + (((uint32_t) k[ 7]) << 24));

		c += (  k[8]			   + (((uint32_t) k[ 9]) <<  8)
		      + (((uint32_t) k[10]) << 16) + (((uint32_t) k[11]) << 24));

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
	return c;
}
#undef mix32

static inline int64_t
hash_string(const unsigned char *const restrict string,
	    const size_t length)
{
	return (int64_t) jhash32(string,
				 length,
				 0xfedcba98);
}
#endif /* if USE_HASH */

struct Key *
pop_key(void)
{
	static struct Key buffer[128];
	static struct Key *restrict alloc	      = &buffer[0];
	static struct Key *const restrict alloc_until = &buffer[128];

	if (alloc < alloc_until) {
		struct Key *const restrict key = alloc;
		++alloc;
		return key;
	}

	EXIT_ON_FAILURE("pop_key failure: out of space");
	__builtin_unreachable();
}


void
make_key(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length)
{
#if USE_HASH
	key->hash = hash_string(string,
				length);
#endif /* if USE_HASH */

	key->string = store_string(string,
				   length);
}

void
key_init(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length)
{
#if USE_HASH
	key->hash = hash_string(string,
				length);
#endif /* if USE_HASH */
	key->string = string;
}

static inline int64_t
string_compare(const unsigned char *restrict string1,
	       const unsigned char *restrict string2)
{
	int token1;
	int token2;

	while (1) {
		token1 = (int) *string1;
		token2 = (int) *string2;

		if (token1 != token2)
			return (int64_t) (token1 - token2);

		if (token1 == 0)
			return 0;

		++string1;
		++string2;
	}
}

int64_t
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2)
{
#if USE_HASH
	return (key1->hash == key2->hash)
	     ? string_compare(key1->string,
			      key2->string)
	     : key1->hash - key2->hash;
#else
	return string_compare(key1->string,
			      key2->string);
#endif /* if USE_HASH */
}


static inline char *
put_string(char *restrict buffer,
	   const char *restrict string)
{
	while (*string != '\0') {
		*buffer = *string;
		++buffer;
		++string;
	}

	return buffer;
}

#if USE_HASH
static inline char *
put_uint(char *const restrict buffer,
	 const uintmax_t n)
{
	return buffer + sprintf(buffer,
				"%zu",
				n);
}
#endif /* if USE_HASH */

char *
put_key(char *restrict buffer,
	const struct Key *const restrict key)
{
#if USE_HASH
	*buffer = '{'; ++buffer;
	*buffer = ' '; ++buffer;

	buffer = put_uint(buffer,
			  (uintmax_t) key->hash);

	*buffer = ':'; ++buffer;
	*buffer = ' '; ++buffer;
#endif /* if USE_HASH */
	*buffer = '"'; ++buffer;

	buffer = put_string(buffer,
			    (const char *) key->string);

	*buffer = '"'; ++buffer;
#if USE_HASH
	*buffer = ' '; ++buffer;
	*buffer = '}'; ++buffer;
#endif /* if USE_HASH */

	return buffer;
}

#if 0
#include <stdio.h>

int
main(void)
{
	struct Key *const restrict key1 = get_key();
	struct Key *const restrict key2 = get_key();

	const int64_t compare = key_compare(key1,
					    key2);



	printf("(%lld) {%lld, %s} is %s {%lld, %s}\n",
	       compare,
	       key1->hash, key1->string,
	       (compare == 0) ? "equal to" : (compare > 0) ? "greater than" : "less than",
	       key2->hash, key2->string);

	return 0;
}
#endif
