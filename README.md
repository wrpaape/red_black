#red_black

##Overview
TODO `red_black`

##Prerequisites

##Build
TODO

##Usage

###Interface

####red_black_tree_init()
**(definition)[#inc/red_black_tree]**
```
void
red_black_tree_init(RedBlackTree *const restrict tree,
                    const RedBlackComparator comparator);
```

**source**


**responsibilities**
- store an empty tree structure
- store key comparator
- initialize node allocator

**notes**
- allocates no memory on the heap until first call to [red_black_tree_insert()](#red_black_tree_insert()), after which [red_black_tree_destroy()](#red_black_tree_destroy()) must be called when done with `tree` to prevent a memory leak


####red_black_tree_destroy()
**(definition)[#inc/red_black_tree]**
```
void
red_black_tree_destroy(RedBlackTree *const restrict tree);
```

**responsibilities**
- frees all memory allocated for `tree`

**notes**
- may only be called after [red_black_tree_init()](#red_black_tree_init())
- safe to call before first insertion
- memory is deallocated via [RED_BLACK_FREE()](#red_black_malloc/red_black_free)


###Types

####RedBlackTree
**(definition)[#inc/red_black_tree]**
```
/* inc/red_black_tree.h */

struct _RedBlackTree {
        struct RedBlackNode *root;
        RedBlackComparator comparator;
        struct RedBlackAllocator allocator;
};

typedef struct _RedBlackTree RedBlackTree;
```

**responsibilities**
* stores
    - root node of the tree structure ([struct RedBlackNode](#struct-redblacknode))
    - key comparator ([RedBlackComparator](#redblackcomparator))
    - node allocator ([struct RedBlackAllocator](#struct-redblackallocator))


###RedBlackComparator
**definition**
```
/* inc/red_black_comparator.h */

typedef int
(*RedBlackComparator)(const void *key1,
                      const void *key2);
```
**responsibilities**
- maintain sorted order of tree nodes

should compare two typeless keys according to the table:
| returns | `key1` __ `key2` |
| ------- | ---------------- |
|   < 0   |        <         |
| ------- | ---------------- |
|    0    |        ==        |
| ------- | ---------------- |
|   > 0   |        >         |

without changing them


###RedBlackTreeIterator
**definition**
```
/* inc/red_black_iterator.h */

struct _RedBlackIterator {
        const struct RedBlackNode *restrict *restrict cursor;
        RedBlackIteratorUpdater update;
        const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};

typedef struct _RedBlackIterator RedBlackIterator;
```

**responsibilities**
- maintain state of iteration across calls to [`red_black_iterator_next`](#red_black_iterator_next)


###RedBlackKeySizer
**definition**
```
/* inc/red_black_print_types.h */

typedef size_t
(*RedBlackKeySizer)(const void *key);
```

**responsibilities**
- provide [`red_black_tree_print`](#red_black_tree_print) with sizing information for buffer allocation

should return the count of ascii characters that are required for [string representation](#redblackkeyputter) of `key` or an overestimation thereof


###RedBlackKeyPutter
**definition**
```
/* inc/red_black_print_types.h */

typedef char *
(*RedBlackKeyPutter)(char *buffer,
                     const void *key);
```
**responsibilities**
- provide [`red_black_tree_print`](#red_black_tree_print) with a means of stringifying keys

should write the string representation of input `key` to `buffer` and return pointer advanced immediately beyond the final character



##Internals

###struct RedBlackNode

###struct RedBlackAllocator


###RED_BLACK_MALLOC, RED_BLACK_FREE
