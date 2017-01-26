#include "red_black_tree.h" /* RedBlackTree|Etor */
#include "str_key.h"        /* str_key_comparator */
#include <stdio.h>          /* puts, perror, EOF */
#include <stdlib.h>         /* exit, EXIT_FAILURE */

/* failure macro */
#define EXIT_ON_FAILURE(FAILURE)				\
do {								\
	perror(FAILURE);					\
	exit(EXIT_FAILURE);					\
} while (0)


int
main(int argc,
     char *argv[])
{
	RedBlackTree tree;
	RedBlackTreeEtor etor;
	const char *restrict key;

	/* initialize string tree */
	red_black_tree_init(&tree,
			    &str_key_comparator);

	/* insert arguments */
	for (int i = 0; i < argc; ++i)
		if (red_black_tree_insert(&tree,
					  argv[i]) < 0)
			EXIT_ON_FAILURE("insertion failure -- OUT OF MEMORY");

	/* initialize ascending etor */
	red_black_tree_asc_etor_init(&etor,
				     &tree);

	/* print unique arguments in ascending order */
	while (red_black_tree_etor_next(&etor,
					(void **) &key))
		if (puts(key) == EOF)
			EXIT_ON_FAILURE("puts failure");

	/* free tree allocations */
	red_black_tree_destroy(&tree);

	return 0;
}
