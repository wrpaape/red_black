#ifndef RED_BLACK_LTREE_H_
#define RED_BLACK_LTREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_lock.h" /* RedBlackLock */
#include "red_black_tree.h" /* RedBlackTree, RedBlackTreeEtor */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackLTree {
	RedBlackLock lock;
	RedBlackTree tree;
};
typedef struct _RedBlackLTree RedBlackLTree;

typedef struct RedBlackEtor RedBlackLTreeEtor;

struct _RedBlackLTreeLEtor {
	RedBlackLTreeEtor tree_etor;
	RedBlackLock *restrict tree_lock;
};
typedef struct _RedBlackLTreeLEtor RedBlackLTreeLEtor;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_ltree_init(RedBlackLTree *const restrict tree,
		     const RedBlackComparator comparator);

#endif /* ifndef RED_BLACK_LTREE_H_ */
