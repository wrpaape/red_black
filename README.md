#red_black



##Overview
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Usage](#usage)
- [Interface](#interface)
- [Types](#types)
- [Internals](#internals)
- [Implementation](#implementation)

TODO `red_black`



##Prerequisites
TODO



##Build
TODO



##Usage
1. [Build](#build) `red_black_tree` libraries
2. `#include` [red_black_tree.h](include/red_black_tree.h) in your source code
3. implement an ordered set or associative array with the provided [interface](#interface)
4. compile and link with either static or shared `red_black_tree` library



##Interface
**Functions**
- [red_black_tree_init](#red_black_tree_init)
- [red_black_tree_destroy](#red_black_tree_destroy)
- [red_black_tree_insert](#red_black_tree_insert)
- [red_black_tree_delete](#red_black_tree_delete)
- [red_black_tree_find](#red_black_tree_find)
- [red_black_tree_count](#red_black_tree_count)
- [red_black_tree_iterator_init_asc](#red_black_tree_iterator_init_asc)
- [red_black_tree_iterator_init_desc](#red_black_tree_iterator_init_desc)
- [red_black_tree_iterator_next](#red_black_tree_iterator_next)
- [red_black_tree_print](#red_black_tree_print)
- [red_black_tree_verify](#red_black_tree_verify)

**Types**
- [RedBlackTree](#redblacktree)
- [RedBlackComparator](#redblackcomparator)
- [RedBlackIterator](#redblackiterator)
- [RedBlackKeySizer](#redblackkeysizer)
- [RedBlackKeyPutter](#redblackkeyputter)



###red_black_tree_init
**[declaration](include/red_black_tree.h#L27-L29)|[source](src/red_black_tree.c#L10-L18)**
```
void
red_black_tree_init(RedBlackTree *const restrict tree,
                    const RedBlackComparator comparator);
```

**description**  
initializes the tree state  
On return `tree` models a valid, empty red-black tree and can safely be operated upon by the remaining interface functions.

**notes**
- allocates no memory on the heap until first call to [red_black_tree_insert](#red_black_tree_insert), after which a call to [red_black_tree_destroy](#red_black_tree_destroy) must be made when done with `tree` to prevent a memory leak


###red_black_tree_destroy
**[declaration](#include/red_black_tree.h#L31-L32)|[source](#src/red_black_tree.c#L20-L24)**
```
void
red_black_tree_destroy(RedBlackTree *const restrict tree);
```

**description**  
frees all memory allocated by `tree`

**notes**
- safe to call before first insertion
- memory is deallocated via [RED_BLACK_FREE](#red_black_malloc/red_black_free)


###red_black_tree_insert
**[declaration](#include/red_black_tree.h#L34-L36)|[source](#src/red_black_tree.c#L26-L42)**
```
int
red_black_tree_insert(RedBlackTree *const restrict tree,
		                  const void *const key);
```

**description**  
attempts to insert `key` into `tree`

| `return` |                                   meaning                                    | tree state |
| :------: | ---------------------------------------------------------------------------- | :--------: |
|    `1`   | successfully inserted unique key `key` into tree                             |  updated   |
|    `0`   | 'key' was already present in tree, no insertion                              | unchanged  |
|   `-1`   | [RED_BLACK_MALLOC](#red_black_malloc/red_black_free) failure - out of memory | unchanged  |

**notes**
- additional memory is allocated occassionally via [RED_BLACK_MALLOC](#red_black_malloc/red_black_free)


###red_black_tree_delete
**[declaration](#include/red_black_tree.h#L38-L40)|[source](#src/red_black_tree.c#L44-L60)**
```
int
red_black_tree_delete(RedBlackTree *const restrict tree,
                      const void *const key);
```

**description**  
attempts to delete `key` from `tree`

| `return` |                     meaning                     | tree state |
| :------: | ----------------------------------------------- | :--------: |
|   `1`    | successfully deleted member key `key` from tree |  updated   |
|   `0`    | 'key' was not present in tree, no deletion      | unchanged  |

**notes**
TODO


###red_black_tree_find
**[declaration](#include/red_black_tree.h#L42-L44)|[source](#src/red_black_tree.c#L62-L69)**
```
bool
red_black_tree_find(const RedBlackTree *const restrict tree,
                    const void *const key);
```

**description**  
attempts to find `key` in `tree`

| `return` |            meaning             |
| :------: | ------------------------------ |
|  `true`  | found member key `key` in tree |
| `false`  | 'key' was not present in tree  |

**notes**
TODO



###red_black_tree_count
**[declaration](#include/red_black_tree.h#L46-L47)|[source](#src/red_black_tree.c#L71-L75)**
```
unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree);
```

**description**  
returns the count of active nodes in `tree`

**notes**
- not a constant time operation, traverses entire tree on every call
- if set size must be tracked throughout the life of `tree`, it is best maintained as an external variable and updated by adding the return values of [red_black_tree_insert](#red_black_tree_insert) (check first that result >= 0!) and [red_black_tree_delete](#red_black_tree_delete) as they are called



###red_black_iterator_init_asc red_black_iterator_init_desc
**[declaration](#include/red_black_tree.h#L49-L55)|[source](#src/red_black_tree.c#L77-L91)**
```
void
red_black_tree_iterator_init_asc(RedBlackTreeIterator *const restrict iterator,
                                 const RedBlackTree *const restrict tree);

void
red_black_tree_iterator_init_desc(RedBlackTreeIterator *const restrict iterator,
                                  const RedBlackTree *const restrict tree);
```

**description**  
*red_black_iterator_init_asc*  
initializes a tree iterator for traversing all member nodes in *ascending* order according to `tree`'s [RedBlackComparator](#redblackcomparator)  
On return, `iterator` may be passed on successive calls to [red_black_tree_iterator_next](#red_black_tree_iterator_next) to retrieve the next largest key, starting with the smallest.

*red_black_iterator_init_desc*  
initializes a tree iterator for traversing all member nodes in *descending* order according to `tree`'s [RedBlackComparator](#redblackcomparator)  
On return, `iterator` may be passed on successive calls to [red_black_tree_iterator_next](#red_black_tree_iterator_next) to retrieve the next smallest key, starting with the largest.

**notes**
- if tree is modified after an iterator has been initialized, undefined behavior may result during iteration
- makes no heap allocations for internal state, no need to pair with a destructor on completion


###red_black_iterator_next
**[declaration](#include/red_black_tree.h#L57-L59)|[source](#src/red_black_tree.c#L93-L99)**
```
bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iterator,
                             const void **const restrict key_ptr);
```

**description**  
attempts to fetch the next ordered key from `iterator`

| `return` |                  meaning                   |
| :------: | ------------------------------------------ |
|  `true`  | `key_ptr` is set to the next key           |
| `false`  | iteration complete, no more keys available |

**notes**
- relies on coupled `tree` remaining unmodified from `iterator`'s initialization until iteration completion


###red_black_tree_print
**[declaration](#include/red_black_tree.h#L61-L64)|[source](#src/red_black_tree.c#L101-L109)**
```
bool
red_black_tree_print(const RedBlackTree *const restrict tree,
                     const RedBlackKeySizer key_sizer,
                     const RedBlackKeyPutter key_putter);
```

**description**  
attempts to print the internal representation of `tree` to standard output
[RedBlackNode](#redblacknode)s are printed line-by-line in ascending order.  Node color and key (string representation for internal nodes, `NULL` for leaves) are positioned at indentation corresponding their node's depth.  For instance, a tree containing the integer keys `1 ... 10` may produce the following:
```
                                                (BLACK) NULL
                                (RED) 1
                                                (BLACK) NULL
                (BLACK) 2
                                                (BLACK) NULL
                                (RED) 3
                                                (BLACK) NULL
(BLACK) 4
                                                                (BLACK) NULL
                                                (RED) 5
                                                                (BLACK) NULL
                                (BLACK) 6
                                                                (BLACK) NULL
                                                (RED) 7
                                                                (BLACK) NULL
                (RED) 8
                                                                (BLACK) NULL
                                                (RED) 9
                                                                (BLACK) NULL
                                (BLACK) 10
                                                (BLACK) NULL
```
**notes**
- see [RedBlackKeySizer](#redblackkeysizer) and [RedBlackKeyPutter](#redblackkeyputter) for correct implementation of input key sizing/putting functions


###red_black_verify
**[declaration](#include/red_black_tree.h#L66-L67)|[source](#src/red_black_tree.c#L111-L116)**
```
bool
red_black_tree_verify(const RedBlackTree *const restrict tree);
```

**description**  
validates that `tree`'s keys are properly ordered and that its nodes adhere to the [red-black tree properties](#red-black-tree-properties)

| `return` |                                            meaning                                             |
| :------: | ---------------------------------------------------------------------------------------------- |
|  `true`  | `tree` is proper                                                                               |
| `false`  | at least one key is out of order and/or at least one red-black tree property has been violated |

**notes**


###RedBlackTree
**[declaration](#include/red_black_tree.h#L14-L20)**
```
struct _RedBlackTree {
        struct RedBlackNode *root;
        RedBlackComparator comparator;
        struct RedBlackAllocator allocator;
};

typedef struct _RedBlackTree RedBlackTree;
```

**responsibilities**  
stores
- root node of the tree structure ([struct RedBlackNode](#struct-redblacknode))
- key comparator ([RedBlackComparator](#redblackcomparator))
- node allocator ([struct RedBlackAllocator](#struct-redblackallocator))


###RedBlackComparator
**[declaration](#include/red_black_comparator.h#L10-L12)**
```
typedef int
(*RedBlackComparator)(const void *key1,
                      const void *key2);
```
**responsibilities**  
maintain proper sorted order of tree nodes

**behaviour**  
should compare two typeless keys according to the table:

| `return` | `key1` __ `key2` |
| :------: | :--------------: |
|  `< 0`   |       `<`        |
|    `0`   |       `==`       |
|  `> 0`   |       `>`        |

without altering their values



###RedBlackTreeIterator
**[declaration](#include/red_black_tree.h#L22)**
```
typedef struct RedBlackIterator RedBlackTreeIterator;
```

**responsibilities**  
- maintain state of iteration across calls to [`red_black_tree_iterator_next`](#red_black_tree_iterator_next)



###RedBlackKeySizer
```
typedef size_t
(*RedBlackKeySizer)(const void *key);
```

**responsibilities**  
- provide [`red_black_tree_print`](#red_black_tree_print) with sizing information for buffer allocation

**behaviour**  
should return the count of `char`s required for [string representation](#redblackkeyputter) of `key` or an overestimation thereof


###RedBlackKeyPutter
```
typedef char *
(*RedBlackKeyPutter)(char *buffer,
                     const void *key);
```
**responsibilities**  
- provide [`red_black_tree_print`](#red_black_tree_print) with a means of stringifying keys

should write the string representation of input `key` to `buffer` and return pointer advanced immediately beyond the final character



##Implementation



##Internals

###struct RedBlackNode

###struct RedBlackAllocator


###RED_BLACK_MALLOC, RED_BLACK_FREE

