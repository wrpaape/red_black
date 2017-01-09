#red_black



##Overview
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Usage](#usage)
- [Interface](#interface)
- [Implementation](#implementation)
- [Internals](#internals)


`red_black` is a collection of C libraries that provide a simple interface for
implementing [(ordered) sets](https://en.wikipedia.org/wiki/Set_(abstract_data_type))
and [associative arrays](https://en.wikipedia.org/wiki/Associative_array) in single and multi-threaded applications.
`red_black` containers are designed from the primitives:
- [red-black tree](https://en.wikipedia.org/wiki/Red-black_tree)
- [hashing](https://en.wikipedia.org/wiki/Hash_function)
- [readers-writer/shared-exclusive lock](https://en.wikipedia.org/wiki/Readers-writer_lock)  

to provide the following operations:

- insert
- update
- find
- delete
- (ordered) traversal  

with competitive [worst-case performance guarantees](https://en.wikipedia.org/wiki/Worst-case_complexity)
and limited [contention](https://en.wikipedia.org/wiki/Lock_(computer_science)#Granularity).
`red_black` distinguishes itself from many other balanced tree/balanced tree-hash table hybrid implementations
by preferring [finer casewise granularity](#implementation) over heavier abstraction throughout container mutations.






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
- [red_black_tree](#red_black_tree)
    - **Functions**
        - [red_black_tree_init](#red_black_tree_init)
        - [red_black_tree_destroy](#red_black_tree_destroy)
        - [red_black_tree_insert red_black_tree_update](#red_black_tree_insert-red_black_tree_update)
        - [red_black_tree_delete red_black_tree_remove](#red_black_tree_delete-red_black_tree_remove)
        - [red_black_tree_find red_black_tree_fetch](#red_black_tree_find-red_black_tree_fetch)
        - [red_black_tree_count](#red_black_tree_count)
        - [red_black_tree_iterator_init_asc red_black_tree_iterator_init_desc](#red_black_tree_iterator_init_asc-red_black_tree_iterator_init_desc)
        - [red_black_tree_iterator_next](#red_black_tree_iterator_next)
        - [red_black_tree_print](#red_black_tree_print)
        - [red_black_tree_verify](#red_black_tree_verify)

    - **Types**
        - [RedBlackTree](#redblacktree)
        - [RedBlackComparator](#redblackcomparator)
        - [RedBlackTreeIterator](#redblacktreeiterator)
        - [RedBlackKeySizer](#redblackkeysizer)
        - [RedBlackKeyPutter](#redblackkeyputter)

- [red_black_hash_map](#red_black_hash_map)
    - **Functions**
        - [red_black_hash_map_init](#red_black_hash_map_init)
        - [red_black_hash_map_destroy](#red_black_hash_map_destroy)
        - [red_black_hash_map_insert red_black_hash_map_update](#red_black_hash_map_insert-red_black_hash_map_update)
        - [red_black_hash_map_delete red_black_hash_map_remove](#red_black_hash_map_delete-red_black_hash_map_remove)
        - [red_black_hash_map_find red_black_hash_map_fetch](#red_black_hash_map_find-red_black_hash_map_fetch)
        - [red_black_hash_map_count](#red_black_hash_map_count)
        - [red_black_hash_map_iterator_init](#red_black_hash_map_iterator_init)
        - [red_black_hash_map_iterator_next](#red_black_hash_map_iterator_next)
        - [red_black_hash_map_verify](#red_black_hash_map_verify)

    - **Types**
        - [RedBlackHashMap](#redblackhashmap)
        - [RedBlackHashMapIterator](#redblackhashmapiterator)



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
- allocates no memory on the heap until first successful [insertion](#red_black_tree_insert), after which a call to [red_black_tree_destroy](#red_black_tree_destroy) must be made when done with `tree` to prevent a memory leak



###red_black_tree_destroy
**[declaration](include/red_black_tree.h#L31-L32)|[source](src/red_black_tree.c#L20-L24)**
```
void
red_black_tree_destroy(RedBlackTree *const restrict tree);
```

**description**  
frees all memory allocated by `tree`

**notes**
- safe to call before first insertion
- memory is deallocated via [RED_BLACK_FREE](#red_black_malloc/red_black_free)



###red_black_tree_insert red_black_tree_update
**[declaration](include/red_black_tree.h#L34-L36)|[source](src/red_black_tree.c#L26-L42)**
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
|   `-1`   | [RED_BLACK_MALLOC](#red_black_malloc-red_black_free) failure - out of memory | unchanged  |

**notes**
- additional memory is allocated occassionally via [RED_BLACK_MALLOC](#red_black_malloc-red_black_free)


###red_black_tree_delete
**[declaration](include/red_black_tree.h#L38-L40)|[source](src/red_black_tree.c#L44-L60)**
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
**[declaration](include/red_black_tree.h#L42-L44)|[source](src/red_black_tree.c#L62-L69)**
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
**[declaration](include/red_black_tree.h#L46-L47)|[source](src/red_black_tree.c#L71-L75)**
```
unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree);
```

**description**  
returns the count of active nodes in `tree`

**notes**
- not a constant time operation, traverses entire tree on every call
- if set size must be tracked throughout the life of `tree`, it is best maintained as an external variable and updated by adding the return values of [red_black_tree_insert](#red_black_tree_insert) (check first that result >= 0!) and [red_black_tree_delete](#red_black_tree_delete) as they are called



###red_black_iterator_init_asc/red_black_iterator_init_desc
**[declaration](include/red_black_tree.h#L49-L55)|[source](src/red_black_tree.c#L77-L91)**
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
**[declaration](include/red_black_tree.h#L57-L59)|[source](src/red_black_tree.c#L93-L99)**
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
**[declaration](include/red_black_tree.h#L61-L64)|[source](src/red_black_tree.c#L101-L109)**
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
**[declaration](include/red_black_tree.h#L66-L67)|[source](src/red_black_tree.c#L111-L116)**
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
**[declaration](include/red_black_tree.h#L14-L20)**
```
struct _RedBlackTree {
        struct RedBlackNode *root;
        RedBlackComparator comparator;
        struct RedBlackAllocator allocator;
};

typedef struct _RedBlackTree RedBlackTree;
```

**responsibility**  
stores
- root node of the tree structure ([struct RedBlackNode](#struct-redblacknode))
- key comparator ([RedBlackComparator](#redblackcomparator))
- node allocator ([struct RedBlackAllocator](#struct-redblackallocator))


###RedBlackComparator
**[declaration](include/red_black_comparator.h#L10-L12)**
```
typedef int
(*RedBlackComparator)(const void *key1,
                      const void *key2);
```
**responsibility**  
maintain proper sorted order of tree nodes

**behaviour**  
should compare two typeless keys according to the table:

| `return` | `key1` __ `key2` |
| :------: | :--------------: |
|  `< 0`   |       `<`        |
|    `0`   |       `==`       |
|  `> 0`   |       `>`        |

without altering their values

**examples**  
```
/* for trees housing integer keys */
#include <stdint.h>	/* intptr_t */
int
compare_integer_keys(const void *key1,
                     const void *key2)
{
        const int int_key1 = (int) (intptr_t) key1;
        const int int_key2 = (int) (intptr_t) key2;

#if 0 /* if domain of keys is known to be small */
        return int_key1 - int_key2;

#else /* if subtraction of keys may cause arithmetic overflow */
        if (int_key1 < int_key2)
                return -1;

        if (int_key1 > int_key2)
                return  1;

        return 0;
#endif
}


/* for trees housing string keys */
int
compare_string_keys(const void *key1,
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
```


###RedBlackTreeIterator
**[declaration](include/red_black_tree.h#L22)**
```
typedef struct RedBlackIterator RedBlackTreeIterator;
```

**responsibility**  
maintain state of iteration across calls to [`red_black_tree_iterator_next`](#red_black_tree_iterator_next)



###RedBlackKeySizer
```
typedef size_t
(*RedBlackKeySizer)(const void *key);
```

**responsibility**  
provide [`red_black_tree_print`](#red_black_tree_print) with sizing information for buffer allocation

**behaviour**  
should return the count of `char`s required for [string representation](#redblackkeyputter) of `key` or an overestimation thereof

**examples**  
```
/* for trees housing integer keys */
#include <stddef.h> /* size_t */
size_t
size_integer_key(const void *key)
{
        return 20; /* enough room for UINT64_MAX ("18446744073709551615") */
}


/* for trees housing string keys */
size_t
size_string_key(const void *key)
{
        const char *string;

        string = (const char *) key;

        while (*string != '\0')
                ++string;

        /* return string length */
        return string - ((const char *) key);
}
```


###RedBlackKeyPutter
```
typedef char *
(*RedBlackKeyPutter)(char *buffer,
                     const void *key);
```
**responsibility**  
provide [`red_black_tree_print`](#red_black_tree_print) with a means of stringifying keys

**behaviour**  
should write the string representation of input `key` to `buffer` and return a pointer advanced immediately beyond the final character  
If a terminating `NUL` byte is written to `buffer`, return a pointer to it so it may be overwritten by the printer.

**examples**  
```
/* for trees housing integer keys */
#include <stdint.h> /* intptr_t */
#define DIGIT_TO_ASCII(DIGIT)	((DIGIT) | 48u) /* 0...9 → '0'...'9' */
char *
put_integer_key(char *buffer,
                const void *key)
{
        int int_key;
        char tmp;
        char *restrict ptr;
        char *restrict end_ptr;

        int_key = (int) (intptr_t) key;

        if (int_key < 0) {
                int_key = -int_key; /* ensure key is non-negative */
                *buffer++ = '-';    /* add sign */
        }

        ptr = buffer;

        /* write digits in reverse order */
        do {
                *ptr++ = (char) DIGIT_TO_ASCII(int_key % 10);
                int_key /= 10;
        } while (int_key > 0);

        end_ptr = ptr; /* copy pointer beyond last digit */

        /* swap digits in place */
        while (--ptr > buffer) {
                tmp       = *ptr;
                *ptr      = *buffer;
                *buffer++ = tmp;
        }

        return end_ptr; /* done */
}


/* for trees housing string keys */
char *
put_string_key(char *buffer,
               const void *key)
{
        const char *string;

        string = (const char *) key;

        /* copy string */
        while (*string != '\0')
                *buffer++ = *string++;

        return buffer;
}
```


##Implementation
TODO





##Internals

###struct RedBlackNode



###struct RedBlackAllocator





###RED_BLACK_MALLOC RED_BLACK_FREE



###RedBlackJumpBuffer RED_BLACK_SET_JUMP RED_BLACK_LONG_JUMP


