#include "red_black_tree.h" /* RedBlackTree|Iterator */
#include <stdio.h>          /* puts, perror */
#include <stdlib.h>         /* exit */

int
compare_strings(const void *key1,
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

int
main(int argc,
     char *argv[])
{
	RedBlackTree tree;
	RedBlackTreeIterator iterator;
	const void *key;

	/* initialize string tree */
	red_black_tree_init(&tree,
			    &compare_strings);

	/* insert arguments */
	for (int i = 0; i < argc; ++i)
		if (red_black_tree_insert(&tree,
					  argv[i]) < 0) {
			perror("insertion failed -- OUT OF MEMORY\n");
			exit(1);
		}

	/* initialize ascending iterator */
	red_black_tree_iterator_init_asc(&iterator,
					 &tree);

	/* print unique arguments in ascending order */
	while (red_black_tree_iterator_next(&iterator,
					    &key))
		puts((const char *) key);

	/* free tree allocations */
	red_black_tree_destroy(&tree);

	return 0;
}
