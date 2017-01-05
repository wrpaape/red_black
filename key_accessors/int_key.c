#include "int_key.h"	/* size_t, bool */
#include <stdint.h>	/* intptr_t */


int
int_key_comparator(const void *key1,
		   const void *key2)
{
	const long int_key1 = (long) (intptr_t) key1;
	const long int_key2 = (long) (intptr_t) key2;

	/* to prevent overflow */
	if (int_key1 < int_key2)
		return -1;

	if (int_key1 > int_key2)
		return  1;

	return 0;
}

inline size_t
uint_digit_count(const unsigned long n)
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
int_key_sizer(const void *key)
{
	long int_key;

	int_key = (long) (intptr_t) key;

	const size_t is_negative = (int_key < 0);

	if (is_negative)
		int_key = -int_key;

	return is_negative + uint_digit_count((unsigned long) int_key);
}


#define DIGIT_TO_ASCII(DIGIT) ((DIGIT) | 48u) /* 0...9 → '0'...'9' */
char *
int_key_putter(char *buffer,
	       const void *key)
{
	long digit;
	long int_key;
	char *restrict end_ptr;

	int_key = (long) (intptr_t) key;

	if (int_key < 0) {
		int_key = -int_key; /* ensure key is non-negative */
		*buffer++ = '-';    /* add sign */
	}

	end_ptr = buffer + uint_digit_count((unsigned long) int_key);
	buffer  = end_ptr;

	do {
		--buffer;

		digit = int_key % 10;
		int_key /= 10;

		*buffer = (char) DIGIT_TO_ASCII(digit);
	} while (int_key > 0);

	return end_ptr;
}
