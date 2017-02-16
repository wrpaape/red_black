#include "unity.h"			   /* UNITY testing framework */
#include "red_black_test.h"		   /* random_upto, keys */
#include "red_black_hmap/red_black_hkey.h" /* HKey, comparator, init */



void
setUp(void)
{
}

void
tearDown(void)
{
}



void
test_red_black_hkey(void)
{
	struct RedBlackHKey static_hkey;
	int *restrict static_key;
	int key_value;
	int *restrict auto_key;
	struct RedBlackHKey auto_hkey;

	static_key = &keys[random_upto(KEYS_COUNT - 1)];

	red_black_hkey_init(&static_hkey,
			    (void *) static_key,
			    sizeof(*static_key));

	key_value = *static_key;

	auto_key = &key_value;

	red_black_hkey_init(&auto_hkey,
			    (void *) auto_key,
			    sizeof(*auto_key));

	const int compare = red_black_hkey_comparator(&auto_hkey,
						      &static_hkey);

	printf("hkeys compared:\n"
	       "   {%lu, %p: %d, %zu}\n"
	       "%s {%lu, %p: %d, %zu}\n",
	       (unsigned long) auto_hkey.hash,
	       auto_hkey.key,
	       *((int *) auto_hkey.key),
	       auto_hkey.length,
	       (compare < 0) ? "< " : ((compare > 0) ? "> " : "=="),
	       (unsigned long) static_hkey.hash,
	       static_hkey.key,
	       *((int *) static_hkey.key),
	       static_hkey.length);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      compare,
				      "KEYS NOT EQUAL");
}
