#ifndef KEY_UTILS_H_
#define KEY_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <unistd.h>	/* read, write */
#include <stdlib.h>	/* exit */
#include <stdint.h>	/* u|int32|64_t */
#include <string.h>	/* memcpy */
#include <stdio.h>	/* sprintf */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(FAILURE)					\
exit_on_failure(FAILURE "\n",						\
		sizeof(FAILURE))

#define EXIT_ON_SUCCESS(SUCCESS)					\
exit_on_success(SUCCESS "\n",						\
		sizeof(SUCCESS))

#define WRITE_STDOUT(BUFFER,						\
		     SIZE)						\
if (write(STDOUT_FILENO,						\
	  BUFFER,							\
	  SIZE) < 0)							\
	EXIT_ON_FAILURE("write failure")


/* toggle on/off string hashing
 * ────────────────────────────────────────────────────────────────────────── */
#define USE_HASH 1

/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Key {
#if USE_HASH
	int64_t hash;
#endif /* if USE_HASH */
	const unsigned char *string;
};

/* limits
 * ────────────────────────────────────────────────────────────────────────── */
#if USE_HASH
#define HASH_MAX (((int64_t) UINT32_MAX) + 1)
#define HASH_MIN (-HASH_MAX)
#endif /* if USE_HASH */
#define STRING_MAX (const unsigned char *) "\xFF"
#define STRING_MIN (const unsigned char *) "\x00"

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern const struct Key KEY_MIN;
extern const struct Key KEY_MAX;

void
exit_on_failure(const char *const restrict failure,
		const size_t length_failure)
__attribute__((noreturn));

void
exit_on_success(const char *const restrict message,
		const size_t length_message)
__attribute__((noreturn));

struct Key *
pop_key(void);

void
make_key(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length);

void
key_init(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length);

int64_t
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2);

char *
put_key(char *restrict buffer,
	const struct Key *const restrict key);

#endif /* ifndef KEY_UTILS_H_ */
