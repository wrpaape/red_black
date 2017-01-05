#include "str_key.h"

int
str_key_comparator(const void *key1,
		   const void *key2)
{
	const unsigned char *string1;
	const unsigned char *string2;
	int token1;
	int token2;

	string1 = (const unsigned char *) key1;
	string2 = (const unsigned char *) key2;

	while (1) {
		token1 = (int) *string1;
		token2 = (int) *string2;

		if (token1 != token2)
			return token1 - token2;

		if (token1 == 0)
			return 0;

		++string1;
		++string2;
	}
}

size_t
str_key_sizer(const void *key)
{
	const char *string;

	string = (const char *) key;

	while (*string != '\0')
		++string;

	/* return string length */
	return string - ((const char *) key);
}

char *
str_key_putter(char *buffer,
	       const void *key)
{
	const char *string;

	string = (const char *) key;

	/* copy string */
	while (*string != '\0')
		*buffer++ = *string++;

	return buffer;
}
