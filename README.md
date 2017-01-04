#red_black



##Overview
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Usage](#usage)
- [Interface](#interface)
- [Types](#types)
- [Internals](#internals)

TODO `red_black`



##Prerequisites
TODO



##Build
TODO



##Usage
1. [Build](#build) `red_black_tree` libraries
2. `#include` [red_black_tree.h](inc/red_black_tree.h) in your source code
3. implement an ordered set or associative array with the provided [interface](#interface)
4. compile and link with either static or shared `red_black_tree` library



##Interface
**[Functions](#functions)**
- [red_black_tree_init](#red_black_tree_init)
- [red_black_tree_destroy](#red_black_tree_destroy)
- [red_black_tree_insert](#red_black_tree_insert)
- [red_black_tree_delete](#red_black_tree_delete)
- [red_black_tree_find](#red_black_tree_find)
- [red_black_tree_count](#red_black_tree_count)
- [red_black_tree_iterator_init_asc](#red_black_tree_iterator_init_asc)
- [red_black_tree_iterator_init_desc](#red_black_tree_iterator_init_desc)
- [red_black_tree_iterator_next](#red_black_tree_iterator_next)
- [red_black_tree_verify](#red_black_tree_verify)
- [red_black_tree_print](#red_black_tree_print)

**[Types](#types)**
- [RedBlackTree](#redblacktree)
- [RedBlackComparator](#redblackcomparator)
- [RedBlackIterator](#redblackiterator)
- [RedBlackKeySizer](#redblackkeysizer)
- [RedBlackKeyPutter](#redblackkeyputter)



###Functions
####red_black_tree_init
```
void
red_black_tree_init(RedBlackTree *const restrict tree,
                    const RedBlackComparator comparator);
```
**[declaration](inc/red_black_tree.h#L27-L29)|[source](src/red_black_tree.c#L10-L18)**

**description**  
initializes the tree state
On return `tree` models a valid, empty red-black tree and can safely be operated upon by the remaining interface.

**notes**
- allocates no memory on the heap until first call to [red_black_tree_insert](#red_black_tree_insert), after which a call to [red_black_tree_destroy](#red_black_tree_destroy) must be made when done with `tree` to prevent a memory leak


####red_black_tree_destroy
```
void
red_black_tree_destroy(RedBlackTree *const restrict tree);
```
**[declaration](#inc/red_black_tree.h#L31-L32)|[source](#src/red_black_tree.c#L20-L24)**

**description**  
frees all memory allocated by `tree`

**notes**
- safe to call before first insertion
- memory is deallocated via [RED_BLACK_FREE](#red_black_malloc/red_black_free)


####red_black_tree_insert
```
int
red_black_tree_insert(RedBlackTree *const restrict tree,
		                  const void *const key);
```
**[declaration](#inc/red_black_tree.h#L34-L36)|[source](#src/red_black_tree.c#L26-L42)**

**description**  
attempts to insert `key` into `tree`

| `return` |                                   meaning                                    | tree state |
| :------: | ---------------------------------------------------------------------------- | :--------: |
|    `1`   | successfully inserted unique key `key` into tree                             |  updated   |
|    `0`   | 'key' was already present in tree, no insertion                              | unchanged  |
|   `-1`   | [RED_BLACK_MALLOC](#red_black_malloc/red_black_free) failure - out of memory | unchanged  |

**notes**
TODO


####red_black_tree_delete
```
bool
red_black_tree_delete(RedBlackTree *const restrict tree,
                      const void *const key);
```
**[declaration](#inc/red_black_tree.h#L38-L40)|[source](#src/red_black_tree.c#L44-L60)**

**description**  
attempts to delete `key` from `tree`

| `return` |                     meaning                     | tree state |
| :------: | ----------------------------------------------- | :--------: |
|  `true`  | successfully deleted member key `key` from tree |  updated   |
| `false`  | 'key' was not present in tree, no deletion      | unchanged  |

**notes**
TODO


####red_black_tree_find
```
bool
red_black_tree_find(const RedBlackTree *const restrict tree,
                    const void *const key);
```
**[declaration](#inc/red_black_tree.h#L42-L44)|[source](#src/red_black_tree.c#L62-L69)**

**description**  
attempts to find `key` in `tree`

| `return` |            meaning             |
| :------: | ------------------------------ |
|  `true`  | found member key `key` in tree |
| `false`  | 'key' was not present in tree  |

**notes**
TODO


###Types

####RedBlackTree
```
/* inc/red_black_tree.h */

struct _RedBlackTree {
        struct RedBlackNode *root;
        RedBlackComparator comparator;
        struct RedBlackAllocator allocator;
};

typedef struct _RedBlackTree RedBlackTree;
```
**[declaration](#inc/red_black_tree.h#L14-L20)**

**responsibilities**
stores
- root node of the tree structure ([struct RedBlackNode](#struct-redblacknode))
- key comparator ([RedBlackComparator](#redblackcomparator))
- node allocator ([struct RedBlackAllocator](#struct-redblackallocator))


####RedBlackComparator
```
typedef int
(*RedBlackComparator)(const void *key1,
                      const void *key2);
```
**responsibilities**
maintain sorted order of tree nodes


**behaviour**
should compare two typeless keys according to the table:

| `return` | `key1` **__** `key2` |
| :------: | :------------------: |
|  `< 0`   |         `<`          |
|    `0`   |         `==`         |
|  `> 0`   |         `>`          |

without altering their values


####RedBlackTreeIterator
```
struct _RedBlackIterator {
        const struct RedBlackNode *restrict *restrict cursor;
        RedBlackIteratorUpdater update;
        const struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
};

typedef struct _RedBlackIterator RedBlackIterator;
```

**responsibilities**
- maintain state of iteration across calls to [`red_black_iterator_next`](#red_black_iterator_next)


####RedBlackKeySizer
**definition**
```
/* inc/red_black_print_types.h */

typedef size_t
(*RedBlackKeySizer)(const void *key);
```

**responsibilities**
- provide [`red_black_tree_print`](#red_black_tree_print) with sizing information for buffer allocation

**behaviour**
should return the count of `char`s required for [string representation](#redblackkeyputter) of `key` or an overestimation thereof


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
