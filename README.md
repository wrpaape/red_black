# red_black



## Overview

- [Prerequisites](#prerequisites)
- [Build](#build)
- [Usage](#usage)
- [Interface](#interface)
- [Implementation](#implementation)
- [Internals](#internals)


`red_black` is a pair of C libraries that provide a simple interface for
implementing [(ordered) sets](https://en.wikipedia.org/wiki/Set_(abstract_data_type))
and [associative arrays](https://en.wikipedia.org/wiki/Associative_array). `red_black`
containers house their data in binary tree structures which are
[height-balanced](https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree)
by adhering to a tried-and-true set of rules--the
[red-black properties](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties).
With proper preservation of these properties, the `RedBlackTree` and `RedBlackHMap`
interfaces are able to provide the following operations:

- insert
- random access
- delete
- (ordered) traverse
- shallow copy
- various set operations

with competitive
[worst-case performance guarantees](https://en.wikipedia.org/wiki/Worst-case_complexity).
The red-black system of balance was chosen to provide greater
responsiveness to mutations than more rigid constructs, such as
[AVL balance factor](https://en.wikipedia.org/wiki/AVL_tree#Balance_factor),
without compromising a *guaranteed* logarithmic height, as is often the case with simpler
methods like [treap priority](https://en.wikipedia.org/wiki/Treap#Description) or
[splaying](https://en.wikipedia.org/wiki/Splay_tree#Splaying).  
`red_black` distinguishes itself from many other balanced tree/hash table-balanced tree
hybrid implementations by preferring [finer casewise granularity](#implementation)
over heavier abstraction throughout container accesses.










## Prerequisites

- GNU Make
- gcc >= 4.8.1 (C99 and C++11 support for benchmarking against `std` containers)
- ruby >= 1.9 (for Unity test runner generator scripts)







## Build

```
$ git clone https://github.com/wrpaape/red_black.git
$ cd red_black
$ make
```
Will build static and shared versions of the `RedBlackTree` and `RedBlackHMap`
libraries in the `static` and `shared` directories, as well as unit tests
and example executables in the `bin` directory.
```
$ make run_tests
```
will run all unit tests, and
```
$ make clean
```
will delete all generated files.



## Usage

1. [Build](#build) the `red_black` libraries
2. If key order is required or memory is constrained,
use a [`RedBlackTree`](#redblacktree-usage).  
If key order is not required and memory can be spared for extra speed,
use a [`RedBlackHMap`](#redblackhmap-usage).


### RedBlackTree Usage

1. Add `#include "red_black_tree.h"` to the top of your source file.  
The file must be included with no path prefix since it will be including other module headers by paths relative to the `include` directory.  
2. Implement an ordered set, associative array, or poor man's priority queue with the provided [interface](#interface).  
Keys are completely opaque to the `RedBlackTree` implementation.  
Accordingly, a [`RedBlackComparator`](#redblackcomparator) must be provided at initialization
to determine ordering.  
Internally keys are stored as `const void *`.  
As a consequence, keys requiring more than `sizeof(void *)` bytes of storage must be
accessible by pointer, and their memory must be managed externally. The same goes for keys
where a access via an `update` or `get` might be useful, as only a *copy* of the actual
stored key will be provided.  
For instance, if the `sizeof(int)` on my machine is 4 bytes, and the `sizeof(void *)` is
at least 8 bytes, then I could store key-value pairs of `int`s directly in my `RedBlackTree`
like so:

    ```
    struct KeyValue {
            int index;
            int counter;
    };
    ...
    struct KeyValue mapping = {
            .index   = 9001,
            .counter = 0
    };
    ...
    int status = red_black_tree_insert(&tree,
                                       *((void **) &mapping));
    assert(status >= 0);
    ```

and not have to worry about their memory management. However, if I wanted to
update the `counter` of a certain member `index`, the following would have no effect
on the stored key:

    ```
    struct KeyValue mapping;
    struct KeyValue fetched;
    ...
    mapping.index = 9001;
    ...
    if (red_black_tree_fetch(&tree,
                             *((void **) &mapping),
                             (void **) &fetched)) {
            /* key with 'index' of 9001 exists in 'tree' */
     
            ++(fetched.counter); /* tree mapping NOT updated!, operates on copy */
    }
    ```

In order to properly update `tree`, I would have to follow up with a `put` and clobber
the old value:

    ```
    if (red_black_tree_fetch(&tree,
                             *((void **) &mapping),
                             (void **) &fetched)) {
            /* key with 'index' of 9001 exists in 'tree' */

            ++(fetched.counter); /* increment counter */

            /* insert into tree, overwriting old value */
            int status = red_black_tree_put(&tree,
                                            *((void **) &fetched));
            assert(status >= 0);
    }
    ```

3. Compile your source code and link to either `static/libred_black_tree.{a,lib}`
or `shared/libred_black_tree.{so,dylib,dll}`.  
Remember to add the `include` directory to your preprocessor's search path
either via compiler flag (i.e. `-Ipath/to/include`) or environment variable
(i.e. `CPATH=path/to/include:$CPATH`).



### RedBlackHMap Usage

1. Add `#include "red_black_hmap.h"` to the top of your source file.  
The file must be included with no path prefix since it will be including other module headers by paths relative to the `include` directory.  
2. Implement an unordered set or associative array with the provided
[interface](#interface).  
`RedBlackHMap` can be thought of as a
[hash table](https://en.wikipedia.org/wiki/Hash_table) of `RedBlackTree`s.  
Accesses are made by hashing input keys of arbitrary `length` to a fixed word,
which is then used to locate the bucket tree where it should reside. Bucket trees
are ordered by this hash word, falling back on an unsigned memory comparison
in case of equal (colliding) values. This method of key organization imposes three
restrictions on the application developer:
    1. Keys **must** be input by their address and `length`.
    2. Key addresses **must** reference valid memory if `length` is nonzero.  
    Keys of zero `length` will emit identical hash values and will not be compared
    by memory. Accordingly, keys of all addresses are considered equivalent to
    `RedBlackHMap` if their `length` is zero, and only one zero-`length` key may reside
    in the container at any time.
    3. Keys **must** be bytewise-distinguishable.  
    Keys of all types are handled as byte strings by `RedBlackHMap`. As a result,
    care must be taken with `struct` keys with fields having different alignments.
    Such `struct`s must be [packed](http://www.catb.org/esr/structure-packing/),
    be cleared before having their fields set, or be passed in such a way that
    for `length` bytes from the input address no padding will be encountered.  
    The following snippet:

    ```
    #define NAME_LENGTH_MAX 20
    #define SPECIES_CAT 0
    ...
    struct Pet {
            char name[NAME_LENGTH_MAX + 1];
            int species;
    };
    ...
    struct Pet *charley_ptr = malloc(sizeof(*charley_ptr));
    assert(charley_ptr != NULL);

    /* insert Charley into set of 'Pet's */
    (void) strcpy(&charley_ptr->name[0],
                  "Charley");
    charley_ptr->species = SPECIES_CAT;

    int status = red_black_hmap_insert(&my_pets,
                                       (void *) charley_ptr,
                                       sizeof(*charley_ptr));
    assert(status >= 0);

    /* check if Charley is a member of 'my_pets' */
    struct Pet charly = {
            .name    = "Charley",
            .species = SPECIES_CAT
    };

    if (red_black_hmap_find(&my_pets,
                            (void *) &charley,
                            sizeof(charley))) {
         /* found Charley */
         ...
    }
    ```

    may exhibit unexpected behavior. The `name` field will not
    always be completely filled for each `Pet` entry, and even if
    all my pets had 20-character (plus one for `'\0'` terminator) names,
    unused padding will still be added after the `name` field to ensure
    that `species` begins on an address aligned to `sizeof(int)`. As such,
    the `find` routine above may fail to retrieve "Charley" the cat from `my_pets`.  
    If there is no need to distinguish `Pet` entries by `species`, the simple solution is to pass the string length of entry `name`s as their key `length`.
    But if I needed to track "Charley" the frog as well I would need to swap the order
    of my `Pet` fields and pass `sizeof(pet.species) + strlen(pet.name)` as key
    `length` to avoid padding bytes, or just `memset(&pet, 0, sizeof(pet))` before
    setting fields and pass `sizeof(pet)` as key `length`.
3. Compile your source code and link to either `static/libred_black_hmap.{a,lib}`
or `shared/libred_black_hmap.{so,dylib,dll}`.  
Remember to add the `include` directory to your preprocessor's search path
either via compiler flag (i.e. `-Ipath/to/include`) or environment variable
(i.e. `CPATH=path/to/include:$CPATH`).









## Interface

- [Creation](#creation)
- [Destruction](#destruction)
- [Insertion](#insertion)
- [Deletion](#deletion)
- [Traversal](#traversal)
- [Random Access](#random-access)
- [Clone](#clone)
- [Inspection](#inspection)
- [Set Comparison](#set-comparison)
- [Set Building Operations](#set-building-operations)


### Creation

**init**
```
void
red_black_tree_init(RedBlackTree *const restrict tree,
                    const RedBlackComparator comparator);

bool
red_black_hmap_init(RedBlackHMap *const restrict map);
```

initialize an empty `red_black` container  
`red_black_tree_init` requires a key comparator
(see [`RedBlackComparator`](#redblackcomparator)) to determine ordering
and performs no heap allocations, so it need not be [destroyed](#destruction) until
the first successful [insertion](#insertion) is made. `red_black_hmap_init` will
need to allocate an array of empty buckets, and so has an opportunity to fail --
a return value of `true` indicates success, `false` indicates failure.


### Destruction

**destroy**
```
void
red_black_tree_destroy(RedBlackTree *const restrict tree);

void
red_black_hmap_destroy(RedBlackHMap *const restrict map);
```
free all internal allocations  
`RedBlackTree` can and `RedBlackHMap` must store its keys by address
and not by copy in order to adequately handle data of all sizes.
Hence, a solution to maintaining persistent keys might be to allocate them
on the heap as they are inserted. If an application loses reference to
such keys, their memory will be leaked when their container is destroyed
unless a final
[traversal](#traversal) is made to free them beforehand.


### Insertion

**insert**
```
int
red_black_tree_insert(RedBlackTree *const restrict tree,
                      const void *const key);

int
red_black_hmap_insert(RedBlackHMap *const restrict map,
                      const void *const key,
                      const size_t length);
```
insert `key` into the container  
If `key` is not already present in the container, an insertion is attempted.
If the insertion succeeds, `1` is returned.  
If the key is already present, the container is untouched and `0` is returned.  
If a memory allocation failure occurs, `-1` is returned.

**add**
```
bool
red_black_tree_add(RedBlackTree *const restrict tree,
                   const void *const key);

bool
red_black_hmap_add(RedBlackHMap *const restrict map,
                   const void *const key,
                   const size_t length);
```
insert **GUARANTEED UNIQUE** `key` into the container  
Some assumptions can be made if `key` is unique that will slightly
speed up insertion, useful for initial provisioning.  
If the insertion succeeds, `true` is returned.  
If a memory allocation failure occurs, `false` is returned.  
This call will segfault or worse if `key` is already present in the container.

**put**
```
int
red_black_tree_put(RedBlackTree *const restrict tree,
                   const void *const key);

int
red_black_hmap_put(RedBlackHMap *const restrict map,
                   const void *const key,
                   const size_t length);
```
insert `key` into the container--if `key` is already present,
overwrite the old value  
If the insertion suceeds, `1` is returned.  
If `key` overwrites a matching key, `0` is returned.  
If a memory allocation failure occurs, `-1` is returned.

**update**
```
int
red_black_tree_update_set(RedBlackTree *const restrict tree,
                          const void *const key,
                          void **const restrict old_ptr);
int
red_black_tree_update_get(RedBlackTree *const restrict tree,
                          const void *const key,
                          void **const restrict old_ptr);

int
red_black_hmap_update_set(RedBlackHMap *const restrict map,
                          const void *const key,
                          const size_t length,
                          void **const restrict old_ptr);
int
red_black_hmap_update_get(RedBlackHMap *const restrict map,
                          const void *const key,
                          const size_t length,
                          void **const restrict old_ptr);
```
insert `key` into the container--if `key` is already present,
set `old_ptr` to the member key's value  
`update_set` will overwrite a matching key in the container with
`key` (like a `put`) before setting `old_ptr`, whereas `update_get`
will leave the container key untouched.  
If the insertion suceeds, `1` is returned, and `old_ptr` is not set.  
If a key matching `key` is present, `0` is returned, and `old_ptr` is set to its value.  
If a memory allocation failure occurs, `-1` is returned.



### Deletion

**delete**
```
int
red_black_tree_delete(RedBlackTree *const restrict tree,
                      const void *const key);
int
red_black_tree_delete_min(RedBlackTree *const restrict tree);
int
red_black_tree_delete_max(RedBlackTree *const restrict tree);

int
red_black_hmap_delete(RedBlackHMap *const restrict map,
                      const void *const key,
                      const size_t length);
```
delete `key` from the container  
If `key` was present in the container, it will be removed and `1` is returned.  
If `key` was not found, the container is untouched and `0` is returned.

**drop**
```
void
red_black_tree_drop(RedBlackTree *const restrict tree,
                    const void *const key);
void
red_black_tree_drop_min(RedBlackTree *const restrict tree);
void
red_black_tree_drop_max(RedBlackTree *const restrict tree);

void
red_black_hmap_drop(RedBlackHMap *const restrict map,
                    const void *const key,
                    const size_t length);
```
delete **GUARANTEED PRESENT** `key` from the container  
Some assumptions can be made if `key` a known member of the container
that will slightly speed up deletion.  
This call will segfault or worse is `key` is not present in the container.


**remove**
```
int
red_black_tree_remove(RedBlackTree *const restrict tree,
                      const void *const key,
                      void **const restrict remove_ptr);
int
red_black_tree_remove_min(RedBlackTree *const restrict tree,
                          void **const restrict remove_ptr);
int
red_black_tree_remove_max(RedBlackTree *const restrict tree,
                          void **const restrict remove_ptr);

int
red_black_hmap_remove(RedBlackHMap *const restrict map,
                      const void *const key,
                      const size_t length,
                      void **const restrict remove_ptr);
```
delete `key` from the container--if `key` was found, set `remove_ptr` to its value  
If `key` was deleted from the container, `1` is returned and `remove_ptr` is set.  
If `key` was not found, the container is untouched, `0` is returned, and `remove_ptr` is not set.

**pluck**
```
void *
red_black_tree_pluck(RedBlackTree *const restrict tree,
                     const void *const key);
void *
red_black_tree_pluck_min(RedBlackTree *const restrict tree);
void *
red_black_tree_pluck_max(RedBlackTree *const restrict tree);

void *
red_black_hmap_pluck(RedBlackHMap *const restrict map,
                     const void *const key,
                     const size_t length);
```
delete **GUARANTEED PRESENT** `key` from the container and return its value  
Some assumptions can be made if `key` a known member of the container
that will slightly speed up deletion.  
This call will segfault or worse is `key` is not present in the container.


### Traversal

**init**
```
void
red_black_tree_asc_itor_init(RedBlackTreeItor *const restrict itor,
                             const RedBlackTree *const restrict tree);
void
red_black_tree_desc_itor_init(RedBlackTreeItor *const restrict itor,
                              const RedBlackTree *const restrict tree);

void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
                         const RedBlackHMap *const restrict map);
```
initialize a traversal `Itor` object to vist all keys of the container  
`RedBlackTreeItor`s may be initialized to traverse in ascending (`asc`, from "smallest" key to "largest")
or descending (`desc`, from "largest" key to "smallest") fashion.  
Container mutations are not allowed throughout the useful lifespan of the `Itor`
(see `next`).  
Neither `Itor` makes internal allocations, so no destructor function need be called
when finished. 

**next**
```
bool
red_black_tree_itor_next(RedBlackTreeItor *const restrict itor,
                         void **const restrict key_ptr);

bool
red_black_hmap_itor_next(RedBlackHMapItor *const restrict itor,
                         void **const restrict key_ptr,
                         size_t *const restrict length_ptr);
```
visit the next key is the container if any remain  
Following an `init` or `reset`, as long as keys remain, `next` will return `true` 
and fetch the next key in the container by setting `key_ptr` (and `length_ptr`).
`false` is returned without setting the input pointer(s) once all member keys
have been traversed.  
Since a stack of unvisted keys its maintained internally, container [insertions](#insertion) and [deletions](#deletion)
must be avoided throughout the useful lifespan of an `Itor`.

**reset**
```
void
red_black_tree_itor_reset(RedBlackTreeItor *const restrict itor,
                          const RedBlackTree *const restrict tree);

void
red_black_hmap_itor_reset(RedBlackHMapItor *const restrict itor,
                          const RedBlackHMap *const restrict map);
```
begin iteration on a new container  
`Itor` must be `init`ialized at least once prior to a `reset`. A `reset` does
not change the direction of iteration for `RedBlackTreeItor`s.


### Random Access

**find**
```
bool
red_black_tree_find(const RedBlackTree *const restrict tree,
                    const void *const key);

bool
red_black_hmap_find(const RedBlackHMap *const restrict map,
                    const void *const key,
                    const size_t length);
```
check if `key` is a member of the container  
`find` returns `true` if `key` was found, `false` if it was not.

**fetch**
```
bool
red_black_tree_fetch(const RedBlackTree *const restrict tree,
                     const void *const key,
                     void **const restrict fetch_ptr);
bool
red_black_tree_fetch_min(const RedBlackTree *const restrict tree,
                         void **const restrict fetch_ptr);
bool
red_black_tree_fetch_max(const RedBlackTree *const restrict tree,
                         void **const restrict fetch_ptr);

bool
red_black_hmap_fetch(const RedBlackHMap *const restrict map,
                     const void *const key,
                     const size_t length,
                     void **const restrict fetch_ptr);
```
attempt to retrieve member `key` from the container  
If `key` is found, store it in `fetch_ptr` and return `true`.  
If `key` is not present, leave `fetch_ptr` unset and return `false`.
`RedBlackTree` provides `fetch_min`/`fetch_max` to retrieve the
smallest/largest key in `tree` without comparisons.

**get**
```
void *
red_black_tree_get(const RedBlackTree *const restrict tree,
                   const void *const key);
void *
red_black_tree_get_min(const RedBlackTree *const restrict tree);
void *
red_black_tree_get_max(const RedBlackTree *const restrict tree);

void *
red_black_hmap_get(const RedBlackHMap *const restrict map,
                   const void *const key,
                   const size_t length);
```
retrieve **GUARANTEED** member `key` from the container  
`get` returns member key comparing equal to input `key` (and `length`).  
`RedBlackTree` provides `get_min`/`get_max` to retrieve the
smallest/largest key in `tree` without comparisons.  
This call will segfault or worse if `key` is not present in the container
or if `tree` is empty in the case of `get_min`/`get_max`.

**replace**
```
bool
red_black_tree_replace(const RedBlackTree *const restrict tree,
                       const void *const key);
bool
red_black_tree_replace_min(const RedBlackTree *const restrict tree,
                           const void *const key);
bool
red_black_tree_replace_max(const RedBlackTree *const restrict tree,
                           const void *const key);

bool
red_black_hmap_replace(const RedBlackHMap *const restrict map,
                       const void *const key,
                       const size_t length);
```
attempt to set member `key` in the container  
If a key is found that matches input `key` (and `length`),
it is replaced by `key` and `true` is returned.  
If no matching key is found, the container is untouched
and `false` is returned.
`RedBlackTree` provides `replace_min`/`replace_max` to set the
smallest/largest key in `tree` without comparisons.

**set**
```
void
red_black_tree_set(const RedBlackTree *const restrict tree,
                   const void *const key);
void
red_black_tree_set_min(const RedBlackTree *const restrict tree,
                       const void *const key);
void
red_black_tree_set_max(const RedBlackTree *const restrict tree,
                       const void *const key);

void
red_black_hmap_set(const RedBlackHMap *const restrict map,
                   const void *const key,
                   const size_t length);
```
set **GUARANTEED** member `key` from the container  
The key matching input `key` (and `length`) is replaced by `key`.  
`RedBlackTree` provides `set_min`/`set_max` to set the
smallest/largest key in `tree` without comparisons.  
This call will segfault or worse if `key` is not present in the container
or if `tree` is empty in the case of `set_min`/`set_max`.

**exchange**
```
bool
red_black_tree_exchange(const RedBlackTree *const restrict tree,
                        const void *const key,
                        void **const restrict old_ptr);
bool
red_black_tree_exchange_min(const RedBlackTree *const restrict tree,
                            const void *const key,
                            void **const restrict old_ptr);
bool
red_black_tree_exchange_max(const RedBlackTree *const restrict tree,
                            const void *const key,
                            void **const restrict old_ptr);

bool
red_black_hmap_exchange(const RedBlackHMap *const restrict map,
                        const void *const key,
                        const size_t length,
                        void **const restrict old_ptr);
```
attempt to swap member `key` in the container  
if a key is found that matches input `key` (and `length`),
it is replaced by `key`, `old_ptr` is set to its old value,
and `true` is returned.  
If no matching key is found, the container is untouched,
`old_ptr` is not set, and `false` is returned.  
`RedBlackTree` provides `exchange_min`/`exchange_max` to swap the
smallest/largest key in `tree` without comparisons.

**swap**
```
void *
red_black_tree_swap(const RedBlackTree *const restrict tree,
                    const void *const key);
void *
red_black_tree_swap_min(const RedBlackTree *const restrict tree,
                        const void *const key);
void *
red_black_tree_swap_max(const RedBlackTree *const restrict tree,
                        const void *const key);

void *
red_black_hmap_swap(const RedBlackHMap *const restrict map,
                    const void *const key,
                    const size_t length);
```
swap **GUARANTEED** member `key` in the container
`swap` will replace the key matching `key` (and `length`)
with `key` and return the old value.  
`RedBlackTree` provides `swap_min`/`swap_max` to swap the
smallest/largest key in `tree` without comparisons.
This call will segfault or worse if `key` is not present in the container.

### Clone

**clone**
```
bool
red_black_tree_clone(RedBlackTree *const restrict dst_tree,
                     const RedBlackTree *const restrict src_tree);

bool
red_black_hmap_clone(RedBlackHMap *const restrict dst_map,
                     const RedBlackHMap *const restrict src_map);
```
allocate a shallow copy of `dst` container and store it in `src`  
If adequate memory can be allocated, the call succeeds and returns `true`.
If memory allocation fails, `false` is returned.  
A successfully `clone`d container is guaranteed to compare `similar` and
`congruent` (see [Set Comparison](set-comparison)) to the original copy.



### Inspection

**empty**
```
bool
red_black_tree_empty(const RedBlackTree *const restrict tree);

bool
red_black_hmap_empty(const RedBlackHMap *const restrict map);
```
check if a container is empty  
If the input container contains no keys, `true` is returned.  
If the input container contains at least one key, `false` is returned.

**count**
```
unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree);

unsigned int
red_black_hmap_count(const RedBlackHMap *const restrict map);
```
query the count of all member keys in the container  
`RedBlackHMap` must maintain its member count to properly manage
its buckets, so `hmap_count` is an `O(1)` operation.  
`RedBlackTree` does not keep track of its member count, so a
full tree traversal is required for `tree_count` (an `O(n)` operation).

**verify**
```
bool
red_black_tree_verify(const RedBlackTree *const restrict tree);

bool
red_black_hmap_verify(const RedBlackHMap *const restrict map);
```
examine the validity of the container state  
`verify` scans all member keys in `O(n)` operations, stopping if
either a pair of keys is found out of (hashed) order or another
one of the [red-black properties](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties) has been violated.  
If the container is in a valid state, `true` is returned.  
If the container is not safe to query/modify, `false` is returned.

### Set Comparison

**similar**
```
bool
red_black_tree_similar(const RedBlackTree *const tree1,
                       const RedBlackTree *const tree2);

bool
red_black_hmap_similar(const RedBlackHMap *const map1,
                       const RedBlackHMap *const map2);
```
check if two containers are *equal*  
If both containers contain exactly the same keys, return `true`.  
If the set of keys differ at all between the two containers, return `false`.

**congruent**
```
bool
red_black_tree_congruent(const RedBlackTree *const tree1,
                         const RedBlackTree *const tree2);

bool
red_black_hmap_congruent(const RedBlackHMap *const map1,
                         const RedBlackHMap *const map2);
```
check if two containers are *identical*  
The `congruent` query checks for container `similar`ity AND equivalent
container shape. Two `congruent` containers will always be `similar`,
but the converse is not necessarily true. For instance a container and
its clone will always compare `similar` AND `congruent`, however,
two `similar` containers may not be `congruent` if their sequence of
key insertions/deletions are not perfectly aligned.  
If both containers contain exactly the same keys and share the same
underlying shape, return `true`.  
If the containers are not `similar` or are shaped differently, return `false`.  

**intersect**
```
bool
red_black_tree_intersect(const RedBlackTree *const tree1,
                         const RedBlackTree *const tree2);

bool
red_black_hmap_intersect(const RedBlackHMap *const map1,
                         const RedBlackHMap *const map2);
```
check if two containers have an overlap in their key set  
If at least one key is found in both containers, return `true`.  
If both containers have completely disjoint key sets, return `false`.

**subset**
```
bool
red_black_tree_subset(const RedBlackTree *const tree1,
                      const RedBlackTree *const tree2);

bool
red_black_hmap_subset(const RedBlackHMap *const map1,
                      const RedBlackHMap *const map2);
```
check if container2 is a subset of container1  
If all of the keys of container2 are members of container1, return `true`.  
If at least one key in container2 is not present in container1, return `false`.


### Set Building Operations

**insert_all**
```
int
red_black_tree_insert_all(RedBlackTree *const restrict dst_tree,
                          const RedBlackTree *const restrict src_tree);

int
red_black_hmap_insert_all(RedBlackHMap *const restrict dst_map,
                          const RedBlackHMap *const restrict src_map);
```
insert all of the keys of `src` container into `dst` container  
On success, `insert_all` returns the count of additional keys inserted.  

If memory allocation fails, `-1` is returned and `dst` is left in an
indeterminate (but valid and destructible) state.

**put_all**
```
int
red_black_tree_put_all(RedBlackTree *const restrict dst_tree,
                       const RedBlackTree *const restrict src_tree);

int
red_black_hmap_put_all(RedBlackHMap *const restrict dst_map,
                       const RedBlackHMap *const restrict src_map);
```
insert all of the keys of `src` container into `dst` container -- matching
keys will be overwritten with those from `src`  
On success, `put_all` returns the count of additional keys inserted.  
If memory allocation fails, `-1` is returned and `dst` is left in an
indeterminate (but valid and destructible) state.

**add_all**
```
bool
red_black_tree_add_all(RedBlackTree *const restrict dst_tree,
                       const RedBlackTree *const restrict src_tree);

bool
red_black_hmap_add_all(RedBlackHMap *const restrict dst_map,
                       const RedBlackHMap *const restrict src_map);
```
insert all **GUARANTEED UNIQUE** keys of `src` container into `dst` container  
Some assumptions can be made if all keys of `src` are known to be unique
(i.e. `intersect(dst, src) == false`) that will slightly speed up insertion.  
If `add_all` succeeds, `true` is returned.  
If a memory allocation failure occurs, `false` is returned.  
This call will segfault or worse if any `src` keys are present in `dst` container.

**delete_all**
```
int
red_black_tree_delete_all(RedBlackTree *const restrict dst_tree,
                          const RedBlackTree *const restrict src_tree);

int
red_black_hmap_delete_all(RedBlackHMap *const restrict dst_map,
                          const RedBlackHMap *const restrict src_map);
```
delete all of the keys of `src` container from `dst` container  
`delete_all` returns the count of overlapping keys deleted.

**drop_all**
```
void
red_black_tree_drop_all(RedBlackTree *const restrict dst_tree,
                        const RedBlackTree *const restrict src_tree);

void
red_black_hmap_drop_all(RedBlackHMap *const restrict dst_map,
                        const RedBlackHMap *const restrict src_map);
```

**union**
```
int
red_black_tree_union(RedBlackTree *const restrict union_tree,
                     const RedBlackTree *const tree1,
                     const RedBlackTree *const tree2);

int
red_black_hmap_union(RedBlackHMap *const restrict union_map,
                     const RedBlackHMap *const map1,
                     const RedBlackHMap *const map2);
```

**intersection**
```
int
red_black_tree_intersection(RedBlackTree *const restrict intersection_tree,
                            const RedBlackTree *const tree1,
                            const RedBlackTree *const tree2);

int
red_black_hmap_intersection(RedBlackHMap *const restrict intersection_map,
                            const RedBlackHMap *const map1,
                            const RedBlackHMap *const map2);
```

**difference**
```
int
red_black_tree_difference(RedBlackTree *const restrict difference_tree,
                          const RedBlackTree *const tree1,
                          const RedBlackTree *const tree2);

int
red_black_hmap_difference(RedBlackHMap *const restrict difference_map,
                          const RedBlackHMap *const map1,
                          const RedBlackHMap *const map2);
```

**sym_difference**
```
int
red_black_tree_sym_difference(RedBlackTree *const restrict sym_difference_tree,
                              const RedBlackTree *const tree1,
                              const RedBlackTree *const tree2);

int
red_black_hmap_sym_difference(RedBlackHMap *const restrict sym_difference_map,
                              const RedBlackHMap *const map1,
                              const RedBlackHMap *const map2);
```


## Types

### RedBlackComparator

**[declaration](include/red_black_tree/red_black_comparator.h#L10-L12)**
```
typedef int
(*RedBlackComparator)(const void *key1,
                      const void *key2);
```
**responsibility**  
maintain proper sorted order of tree nodes

**behaviour**  
should compare two keys according to the table:

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

## Implementation

### The "Parent" Pointer Compromise

TODO
