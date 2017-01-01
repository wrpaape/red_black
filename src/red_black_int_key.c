#include "red_black_int_key.h"	/* uintmax_t */
#include <stdint.h>		/* intptr_t, uintmax_t */
#include <stdlib.h>		/* strtol, NULL */
#include <stdio.h>		/* sprintf */
#include <errno.h>		/* errno */


bool
red_black_int_key_init(void *restrict *const restrict key,
		       const char *const restrict string)
{
	*key = (void *) (intptr_t) strtol(string,
					  NULL,
					  10);

	return (*key != 0) || (errno == 0);
}


int
red_black_int_key_comparator(const void *key1,
			     const void *key2)
{
	const intptr_t int_key1 = (intptr_t) key1;
	const intptr_t int_key2 = (intptr_t) key2;

	/* to prevent overflow */
	if (int_key1 < int_key2)
		return -1;

	if (int_key1 > int_key2)
		return  1;

	return 0;
}

inline unsigned int
uint_digit_count(const uintmax_t n)
{
	if (n < 10000000000llu) {
		if (n < 100000llu) {
			if (n < 1000llu) {
				if (n < 100llu) {
					if (n < 10llu) {
						return  1;
					} else {
						return  2;
					}
				} else {
					return  3;
				}

			} else {
				if (n < 10000llu) {
					return  4;
				} else {
					return  5;
				}
			}
		} else {
			if (n < 100000000llu) {
				if (n < 10000000llu) {
					if (n < 1000000llu) {
						return  6;
					} else {
						return  7;
					}
				} else {
					return  8;
				}
			} else {
				if (n < 1000000000llu)	{
					return  9;
				} else {
					return 10;
				}
			}
		}
	} else {
		if (n < 1000000000000000llu) {
			if (n < 1000000000000llu) {
				if (n < 100000000000llu) {
					return 11;
				} else {
					return 12;
				}
			} else {
				if (n < 10000000000000llu) {
					return 13;
				} else {
					if (n < 100000000000000llu) {
						return 14;
					} else {
						return 15;
					}
				}
			}
		} else {
			if (n < 100000000000000000llu) {
				if (n < 10000000000000000llu) {
					return 16;
				} else {
					return 17;
				}
			} else {
				if (n < 1000000000000000000llu) {
					return 18;
				} else {
					if (n < 10000000000000000000llu) {
						return 19;
					} else {
						return 20;
					}
				}
			}
		}
	}
}

size_t
red_black_int_key_sizer(const void *key)
{
	const intptr_t int_key = (intptr_t) key;

	return (int_key < 0)
	     ? (uint_digit_count(-int_key) + 1)
	     : uint_digit_count(int_key);
}

char *
red_black_int_key_putter(char *buffer,
			 const void *key)
{
	return buffer + sprintf(buffer,
				"%ld",
				(long) (intptr_t) key);
}
