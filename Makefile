# DIRECTORY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
SRC_DIR      := src# .c source files
INCLUDE_DIR  := include# .h header files
OBJ_DIR      := obj# .o object files
BIN_DIR      := bin# binary executable files
LIB_DIR      := lib# static and shared library files
TEST_DIR     := test# test source and header files
EXAMPLES_DIR := examples# example code
DEMO_DIR     := demo# demo source and header files


# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
CC	 := gcc
CC_FLAGS := -std=gnu99 -I$(INCLUDE_DIR) -Wall -O2 -funroll-loops -c
LD	 := ld
LD_FLAGS := -arch x86_64 -macosx_version_min 10.11.0 -lc
RM	 := rm
RM_FLAGS := -rf
AR       := ar
AR_FLAGS := rcs


# HEADER FILES
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_H := $(INCLUDE_DIR)/red_black_stack_count.h
ALLOCATOR_H   := $(INCLUDE_DIR)/red_black_allocator.h
COMPARATOR_H  := $(INCLUDE_DIR)/red_black_comparator.h
ITERATOR_H    := $(INCLUDE_DIR)/red_black_iterator.h
DELETE_H      := $(INCLUDE_DIR)/red_black_delete.h
FIND_H        := $(INCLUDE_DIR)/red_black_find.h
INSERT_H      := $(INCLUDE_DIR)/red_black_insert.h
INT_KEY_H     := $(INCLUDE_DIR)/red_black_int_key.h
JUMP_H        := $(INCLUDE_DIR)/red_black_jump.h
LOCK_H        := $(INCLUDE_DIR)/red_black_lock.h
MALLOC_H      := $(INCLUDE_DIR)/red_black_malloc.h
NODE_H        := $(INCLUDE_DIR)/red_black_node.h
PRINT_H       := $(INCLUDE_DIR)/red_black_print.h
PRINT_TYPES_H := $(INCLUDE_DIR)/red_black_print_types.h
TREE_H        := $(INCLUDE_DIR)/red_black_tree.h
VERIFY_H      := $(INCLUDE_DIR)/red_black_verify.h
COUNT_H       := $(INCLUDE_DIR)/red_black_count.h


# SOURCE FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_C := $(SRC_DIR)/red_black_allocator.c
ITERATOR_C  := $(SRC_DIR)/red_black_iterator.c
DELETE_C    := $(SRC_DIR)/red_black_delete.c
FIND_C      := $(SRC_DIR)/red_black_find.c
INSERT_C    := $(SRC_DIR)/red_black_insert.c
INT_KEY_C   := $(SRC_DIR)/red_black_int_key.c
PRINT_C     := $(SRC_DIR)/red_black_print.c
TREE_C      := $(SRC_DIR)/red_black_tree.c
VERIFY_C    := $(SRC_DIR)/red_black_verify.c
COUNT_C     := $(SRC_DIR)/red_black_count.c
DEMO_C      := $(SRC_DIR)/red_black_demo.c


# TEST FILES
# ──────────────────────────────────────────────────────────────────────────────
TEST_H := $(TEST_DIR)/red_black_test.h
TEST_C := $(TEST_DIR)/red_black_test.c


# EXAMPLE FILES
# ──────────────────────────────────────────────────────────────────────────────
USORT_C := $(EXAMPLES_DIR)/red_black_usort.c


# DEMO FILES
# ──────────────────────────────────────────────────────────────────────────────
DEMO_C        := $(DEMO_DIR)/red_black_demo.c
DEMO_H        := $(DEMO_DIR)/red_black_demo.h

# OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_O := $(OBJ_DIR)/red_black_allocator.o
ITERATOR_O  := $(OBJ_DIR)/red_black_iterator.o
DELETE_O    := $(OBJ_DIR)/red_black_delete.o
FIND_O      := $(OBJ_DIR)/red_black_find.o
INSERT_O    := $(OBJ_DIR)/red_black_insert.o
INT_KEY_O   := $(OBJ_DIR)/red_black_int_key.o
PRINT_O     := $(OBJ_DIR)/red_black_print.o
COUNT_O     := $(OBJ_DIR)/red_black_count.o
TREE_O      := $(OBJ_DIR)/red_black_tree.o
VERIFY_O    := $(OBJ_DIR)/red_black_verify.o
DEMO_O      := $(OBJ_DIR)/red_black_demo.o
TEST_O      := $(OBJ_DIR)/red_black_test.o
USORT_O     := $(OBJ_DIR)/red_black_usort.o


# PIC OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_PO := $(OBJ_DIR)/red_black_allocator_pic.o
ITERATOR_PO  := $(OBJ_DIR)/red_black_iterator_pic.o
DELETE_PO    := $(OBJ_DIR)/red_black_delete_pic.o
FIND_PO      := $(OBJ_DIR)/red_black_find_pic.o
INSERT_PO    := $(OBJ_DIR)/red_black_insert_pic.o
PRINT_PO     := $(OBJ_DIR)/red_black_print_pic.o
COUNT_PO     := $(OBJ_DIR)/red_black_count_pic.o
VERIFY_PO    := $(OBJ_DIR)/red_black_verify_pic.o


# BINARY FILES
# ──────────────────────────────────────────────────────────────────────────────
DEMO  := $(BIN_DIR)/red_black_demo
TEST  := $(BIN_DIR)/red_black_test
USORT := $(BIN_DIR)/red_black_usort


# STATIC LIBRARY FILES
# ──────────────────────────────────────────────────────────────────────────────
TREE_ST := $(LIB_DIR)/red_black_tree.a


# SHARED LIBRARY FILES
# ──────────────────────────────────────────────────────────────────────────────
TREE_SH := $(LIB_DIR)/red_black_tree.dylib


# ALL TARGETS
# ──────────────────────────────────────────────────────────────────────────────
TARGETS := $(USORT) $(USORT_O) $(TREE_ST) $(TREE_SH) $(TREE_O) \
	   $(TEST) $(TEST_O) $(DEMO) $(DEMO_O) \
	   $(ALLOCATOR_O) $(DELETE_O) $(FIND_O) $(INSERT_O) \
	   $(INT_KEY_O) $(PRINT_O) $(VERIFY_O) $(COUNT_O) \
           $(ALLOCATOR_PO) $(DELETE_PO) $(FIND_PO) $(INSERT_PO) \
	   $(PRINT_PO) $(VERIFY_PO) $(COUNT_PO)


# DEPENDENCIES
# ──────────────────────────────────────────────────────────────────────────────
INT_KEY_O_DEP    := $(INT_KEY_C) $(INT_KEY_H)
INT_KEY_O_GRP    := $(INT_KEY_O)

COUNT_O_DEP      := $(COUNT_C) $(COUNT_H) $(NODE_H) $(STACK_COUNT_H)
COUNT_O_GRP      := $(COUNT_O)
COUNT_PO_DEP     := $(COUNT_O_DEP)
COUNT_PO_GRP     := $(COUNT_PO)

VERIFY_O_DEP     := $(VERIFY_C) $(VERIFY_H) $(NODE_H) $(COMPARATOR_H)
VERIFY_O_GRP     := $(VERIFY_O)
VERIFY_PO_DEP    := $(VERIFY_O_DEP)
VERIFY_PO_GRP    := $(VERIFY_PO)

PRINT_O_DEP      := $(PRINT_C) $(PRINT_H) $(NODE_H) $(PRINT_TYPES_H) $(MALLOC_H)
PRINT_O_GRP      := $(PRINT_O)
PRINT_PO_DEP     := $(PRINT_O_DEP)
PRINT_PO_GRP     := $(PRINT_PO)

FIND_O_DEP       := $(FIND_C) $(FIND_H) $(NODE_H) $(COMPARATOR_H)
FIND_O_GRP       := $(FIND_O)
FIND_PO_DEP      := $(FIND_O_DEP)
FIND_PO_GRP      := $(FIND_PO)

ALLOCATOR_O_DEP  := $(ALLOCATOR_C) $(ALLOCATOR_H) $(NODE_H) $(JUMP_H) $(MALLOC_H)
ALLOCATOR_O_GRP  := $(ALLOCATOR_O)
ALLOCATOR_PO_DEP := $(ALLOCATOR_O_DEP)
ALLOCATOR_PO_GRP := $(ALLOCATOR_PO)

ITERATOR_O_DEP   := $(ITERATOR_C) $(ITERATOR_H) $(NODE_H) $(STACK_COUNT_H)
ITERATOR_O_GRP   := $(ITERATOR_O)
ITERATOR_PO_DEP  := $(ITERATOR_O_DEP)
ITERATOR_PO_GRP  := $(ITERATOR_PO)

INSERT_O_DEP     := $(INSERT_C) $(INSERT_H) $(COMPARATOR_H) $(ALLOCATOR_H)
INSERT_O_GRP     := $(INSERT_O) $(ALLOCATOR_O_GRP)
INSERT_PO_DEP    := $(INSERT_O_DEP)
INSERT_PO_GRP    := $(INSERT_PO) $(ALLOCATOR_PO_GRP)

DELETE_O_DEP     := $(DELETE_C) $(DELETE_H) $(COMPARATOR_H) $(ALLOCATOR_H) $(STACK_COUNT_H)
DELETE_O_GRP     := $(DELETE_O) $(ALLOCATOR_O_GRP)
DELETE_PO_DEP    := $(DELETE_O_DEP)
DELETE_PO_GRP    := $(DELETE_PO) $(ALLOCATOR_PO_GRP)

TREE_O_DEP       := $(TREE_C) $(TREE_H) $(COMPARATOR_H) $(ALLOCATOR_H) \
	            $(ITERATOR_H) $(PRINT_TYPES_H) $(INSERT_H) $(DELETE_H) \
	            $(FIND_H) $(VERIFY_H) $(PRINT_H) $(COUNT_H)
TREE_O_GRP       := $(TREE_O) $(ALLOCATOR_O_GRP) $(INSERT_O_GRP) $(DELETE_O_GRP) \
	            $(FIND_O_GRP) $(VERIFY_O_GRP) $(PRINT_O_GRP) $(COUNT_O_GRP) $(ITERATOR_O_GRP)
TREE_PO_DEP      := $(TREE_O_DEP)
TREE_PO_GRP      := $(TREE_PO) $(ALLOCATOR_PO_GRP) $(INSERT_PO_GRP) $(DELETE_PO_GRP) \
	            $(FIND_PO_GRP) $(VERIFY_PO_GRP) $(PRINT_PO_GRP) $(COUNT_PO_GRP) $(ITERATOR_PO_GRP)
TREE_ST_DEP      := $(TREE_O_GRP)
TREE_SH_DEP      := $(TREE_PO_GRP)

DEMO_O_DEP       := $(DEMO_C) $(DEMO_H) $(TREE_H) $(INT_KEY_H)
DEMO_O_GRP       := $(DEMO_O) $(TREE_O_GRP) $(INT_KEY_O_GRP)

DEMO_DEP         := $(DEMO_O) $(DEMO_O_GRP)

TEST_O_DEP       := $(TEST_C) $(TEST_H) $(TREE_H) $(INT_KEY_H)
TEST_O_GRP       := $(TEST_O) $(TREE_O_GRP) $(INT_KEY_O_GRP)

TEST_DEP         := $(TEST_O) $(TEST_O_GRP)

USORT_O_DEP      := $(USORT_C) $(TREE_H)
USORT_DEP        := $(USORT_O) $(TREE_ST)


# MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
all: $(TARGETS)

$(USORT): $(USORT_DEP)
	$(LD) $^ $(LD_FLAGS) -no_pie -o $@

$(TREE_ST): $(TREE_ST_DEP)
	$(AR) $(AR_FLAGS) $@ $^

$(TREE_SH): $(TREE_SH_DEP)
	$(LD) $^ $(LD_FLAGS) -dylib -o $@

$(TEST): $(TEST_DEP)
	$(LD) $^ $(LD_FLAGS) -no_pie -o $@

$(DEMO): $(DEMO_DEP)
	$(LD) $^ $(LD_FLAGS) -no_pie -o $@

$(USORT_O): $(USORT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TEST_O): $(TEST_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(DEMO_O): $(DEMO_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TREE_O): $(TREE_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TREE_PO): $(TREE_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(DELETE_O): $(DELETE_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(DELETE_PO): $(DELETE_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(INSERT_O): $(INSERT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(INSERT_PO): $(INSERT_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(ALLOCATOR_O): $(ALLOCATOR_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(ALLOCATOR_PO): $(ALLOCATOR_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(ITERATOR_O): $(ITERATOR_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(ITERATOR_PO): $(ITERATOR_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(FIND_O): $(FIND_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(FIND_PO): $(FIND_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(PRINT_O): $(PRINT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(PRINT_PO): $(PRINT_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(VERIFY_O): $(VERIFY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(VERIFY_PO): $(VERIFY_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(COUNT_O): $(COUNT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(COUNT_PO): $(COUNT_PO_DEP)
	$(CC) $(CC_FLAGS) -fpic $< -o $@

$(INT_KEY_O): $(INT_KEY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)
