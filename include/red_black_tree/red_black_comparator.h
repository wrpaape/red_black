#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_COMPARATOR_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_COMPARATOR_H_

/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
/* returns
 *	< 0	key1 <  key2
 *	  0	key1 == key2
 *	> 0	key1 >  key2 */
typedef int
(*RedBlackComparator)(const void *key1,
		      const void *key2);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_COMPARATOR_H_ */
