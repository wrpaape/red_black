# DIRECTORY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib


# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
CC	 := gcc
CC_FLAGS := -std=gnu99 -I$(INC_DIR) -Wall -O2 -funroll-loops -c
LD	 := ld
LD_FLAGS := -arch x86_64 -macosx_version_min 10.11.0 -lc
RM	 := rm
RM_FLAGS := -rf
AR       := ar
AR_FLAGS := rcs


# HEADER FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_H   := $(INC_DIR)/red_black_allocator.h
COMPARATOR_H  := $(INC_DIR)/red_black_comparator.h
DELETE_H      := $(INC_DIR)/red_black_delete.h
FIND_H        := $(INC_DIR)/red_black_find.h
INSERT_H      := $(INC_DIR)/red_black_insert.h
INT_KEY_H     := $(INC_DIR)/red_black_int_key.h
JUMP_H        := $(INC_DIR)/red_black_jump.h
LOCK_H        := $(INC_DIR)/red_black_lock.h
MALLOC_H      := $(INC_DIR)/red_black_malloc.h
STACK_COUNT_H := $(INC_DIR)/red_black_stack_count.h
NODE_H        := $(INC_DIR)/red_black_node.h
PRINT_H       := $(INC_DIR)/red_black_print.h
PRINT_TYPES_H := $(INC_DIR)/red_black_print_types.h
TREE_H        := $(INC_DIR)/red_black_tree.h
VERIFY_H      := $(INC_DIR)/red_black_verify.h
DEMO_H        := $(INC_DIR)/red_black_demo.h
TEST_H        := $(INC_DIR)/red_black_test.h


# SOURCE FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_C := $(SRC_DIR)/red_black_allocator.c
DELETE_C    := $(SRC_DIR)/red_black_delete.c
FIND_C      := $(SRC_DIR)/red_black_find.c
INSERT_C    := $(SRC_DIR)/red_black_insert.c
INT_KEY_C   := $(SRC_DIR)/red_black_int_key.c
PRINT_C     := $(SRC_DIR)/red_black_print.c
TREE_C      := $(SRC_DIR)/red_black_tree.c
VERIFY_C    := $(SRC_DIR)/red_black_verify.c
DEMO_C      := $(SRC_DIR)/red_black_demo.c
TEST_C      := $(SRC_DIR)/red_black_test.c


# OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_O := $(OBJ_DIR)/red_black_allocator.o
DELETE_O    := $(OBJ_DIR)/red_black_delete.o
FIND_O      := $(OBJ_DIR)/red_black_find.o
INSERT_O    := $(OBJ_DIR)/red_black_insert.o
INT_KEY_O   := $(OBJ_DIR)/red_black_int_key.o
PRINT_O     := $(OBJ_DIR)/red_black_print.o
TREE_O      := $(OBJ_DIR)/red_black_tree.o
VERIFY_O    := $(OBJ_DIR)/red_black_verify.o
DEMO_O      := $(OBJ_DIR)/red_black_demo.o
TEST_O      := $(OBJ_DIR)/red_black_test.o


# PIC OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_PO := $(OBJ_DIR)/red_black_allocator_pic.o
DELETE_PO    := $(OBJ_DIR)/red_black_delete_pic.o
FIND_PO      := $(OBJ_DIR)/red_black_find_pic.o
INSERT_PO    := $(OBJ_DIR)/red_black_insert_pic.o
PRINT_PO     := $(OBJ_DIR)/red_black_print_pic.o
VERIFY_PO    := $(OBJ_DIR)/red_black_verify_pic.o


# BINARY FILES
# ──────────────────────────────────────────────────────────────────────────────
DEMO := $(BIN_DIR)/red_black_demo
TEST := $(BIN_DIR)/red_black_test


# STATIC LIBRARY FILES
# ──────────────────────────────────────────────────────────────────────────────
TREE_ST := $(LIB_DIR)/red_black_tree.a


# SHARED LIBRARY FILES
# ──────────────────────────────────────────────────────────────────────────────
TREE_SH := $(LIB_DIR)/red_black_tree.dylib


# ALL TARGETS
# ──────────────────────────────────────────────────────────────────────────────
TARGETS := $(TREE_ST) $(TREE_SH) $(TREE_O) $(TEST) $(TEST_O) $(DEMO) $(DEMO_O) \
	   $(ALLOCATOR_O) $(DELETE_O) $(FIND_O) $(INSERT_O) \
	   $(INT_KEY_O) $(PRINT_O) $(VERIFY_O) \
           $(ALLOCATOR_PO) $(DELETE_PO) $(FIND_PO) $(INSERT_PO) \
	   $(PRINT_PO) $(VERIFY_PO)


# DEPENDENCIES
# ──────────────────────────────────────────────────────────────────────────────
INT_KEY_O_DEP    := $(INT_KEY_C) $(INT_KEY_H)
INT_KEY_O_GRP    := $(INT_KEY_O)

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

INSERT_O_DEP     := $(INSERT_C) $(INSERT_H) $(COMPARATOR_H) $(ALLOCATOR_H)
INSERT_O_GRP     := $(INSERT_O) $(ALLOCATOR_O_GRP)
INSERT_PO_DEP    := $(INSERT_O_DEP)
INSERT_PO_GRP    := $(INSERT_PO) $(ALLOCATOR_PO_GRP)

DELETE_O_DEP     := $(DELETE_C) $(DELETE_H) $(COMPARATOR_H) $(ALLOCATOR_H) $(STACK_COUNT_H)
DELETE_O_GRP     := $(DELETE_O) $(ALLOCATOR_O_GRP)
DELETE_PO_DEP    := $(DELETE_O_DEP)
DELETE_PO_GRP    := $(DELETE_PO) $(ALLOCATOR_PO_GRP)

TREE_O_DEP       := $(TREE_C) $(TREE_H) $(COMPARATOR_H) $(ALLOCATOR_H) \
	            $(PRINT_TYPES_H) $(INSERT_H) $(DELETE_H) $(FIND_H) \
		    $(VERIFY_H) $(PRINT_H)
TREE_O_GRP       := $(TREE_O) $(INSERT_O_GRP) $(DELETE_O_GRP) $(FIND_O_GRP) \
	            $(VERIFY_O_GRP) $(PRINT_O_GRP)
TREE_PO_DEP      := $(TREE_O_DEP)
TREE_PO_GRP      := $(TREE_PO) $(INSERT_PO_GRP) $(DELETE_PO_GRP) $(FIND_PO_GRP) \
	            $(VERIFY_PO_GRP) $(PRINT_PO_GRP)
TREE_ST_DEP      := $(TREE_O_GRP)
TREE_SH_DEP      := $(TREE_PO_GRP)

DEMO_O_DEP       := $(DEMO_C) $(DEMO_H) $(TREE_H) $(INT_KEY_H)
DEMO_O_GRP       := $(DEMO_O) $(TREE_O_GRP) $(INT_KEY_O_GRP)

DEMO_DEP         := $(DEMO_O) $(DEMO_O_GRP)

TEST_O_DEP       := $(TEST_C) $(TEST_H) $(TREE_H) $(INT_KEY_H)
TEST_O_GRP       := $(TEST_O) $(TREE_O_GRP) $(INT_KEY_O_GRP)

TEST_DEP         := $(TEST_O) $(TEST_O_GRP)


# MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
all: $(TARGETS)

$(TREE_ST): $(TREE_ST_DEP)
	$(AR) $(AR_FLAGS) $@ $^

$(TREE_SH): $(TREE_SH_DEP)
	$(LD) $^ $(LD_FLAGS) -dylib -o $@

$(TEST): $(TEST_DEP)
	$(LD) $^ $(LD_FLAGS) -no_pie -o $@

$(DEMO): $(DEMO_DEP)
	$(LD) $^ $(LD_FLAGS) -no_pie -o $@

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

$(INT_KEY_O): $(INT_KEY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)
