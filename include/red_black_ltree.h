#ifndef RED_BLACK_LTREE_H_
#define RED_BLACK_LTREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_lock.h" /* RedBlackLock */
#include "red_black_tree.h" /* RedBlackTree, RedBlackTreeIterator */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackLTree {
	RedBlackLock lock;
	RedBlackTree tree;
};
typedef struct _RedBlackLTree RedBlackLTree;

typedef struct RedBlackIterator RedBlackLTreeIterator;

struct _RedBlackLTreeLIterator {
	RedBlackLTreeIterator tree_iterator;
	RedBlackLock *restrict tree_lock;
};
typedef struct _RedBlackLTreeLIterator RedBlackLTreeLIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator);

#endif /* ifndef RED_BLACK_LTREE_H_ */
