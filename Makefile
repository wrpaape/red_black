# DIRECTORY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin


# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
CC	 := gcc
CC_FLAGS := -std=gnu99 -I$(INC_DIR) -Wall -O2 -c
LD	 := ld
LD_FLAGS := -arch x86_64 -macosx_version_min 10.11 -no_pie -lc
RM	 := rm
RM_FLAGS := -rf


# HEADER FILES
# ──────────────────────────────────────────────────────────────────────────────
MAIN_H        := $(INC_DIR)/red_black.h
ALLOCATOR_H   := $(INC_DIR)/red_black_allocator.h
COMPARATOR_H  := $(INC_DIR)/red_black_comparator.h
DELETE_H      := $(INC_DIR)/red_black_delete.h
FIND_H        := $(INC_DIR)/red_black_find.h
INSERT_H      := $(INC_DIR)/red_black_insert.h
INT_KEY_H     := $(INC_DIR)/red_black_int_key.h
JUMP_H        := $(INC_DIR)/red_black_jump.h
LOCK_H        := $(INC_DIR)/red_black_lock.h
MALLOC_H      := $(INC_DIR)/red_black_malloc.h
NODE_H        := $(INC_DIR)/red_black_node.h
PRINT_H       := $(INC_DIR)/red_black_print.h
PRINT_TYPES_H := $(INC_DIR)/red_black_print_types.h
TREE_H        := $(INC_DIR)/red_black_tree.h
VERIFY_H      := $(INC_DIR)/red_black_verify.h


# SOURCE FILES
# ──────────────────────────────────────────────────────────────────────────────
MAIN_C      := $(SRC_DIR)/red_black.c
ALLOCATOR_C := $(SRC_DIR)/red_black_allocator.c
DELETE_C    := $(SRC_DIR)/red_black_delete.c
FIND_C      := $(SRC_DIR)/red_black_find.c
INSERT_C    := $(SRC_DIR)/red_black_insert.c
INT_KEY_C   := $(SRC_DIR)/red_black_int_key.c
PRINT_C     := $(SRC_DIR)/red_black_print.c
TREE_C      := $(SRC_DIR)/red_black_tree.c
VERIFY_C    := $(SRC_DIR)/red_black_verify.c


# OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
MAIN_O      := $(OBJ_DIR)/red_black.o
ALLOCATOR_O := $(OBJ_DIR)/red_black_allocator.o
DELETE_O    := $(OBJ_DIR)/red_black_delete.o
FIND_O      := $(OBJ_DIR)/red_black_find.o
INSERT_O    := $(OBJ_DIR)/red_black_insert.o
INT_KEY_O   := $(OBJ_DIR)/red_black_int_key.o
PRINT_O     := $(OBJ_DIR)/red_black_print.o
TREE_O      := $(OBJ_DIR)/red_black_tree.o
VERIFY_O    := $(OBJ_DIR)/red_black_verify.o


# BINARY FILES
# ──────────────────────────────────────────────────────────────────────────────
MAIN := $(BIN_DIR)/red_black


# ALL TARGETS
# ──────────────────────────────────────────────────────────────────────────────
TARGETS := $(MAIN) $(MAIN_O) $(ALLOCATOR_O) $(DELETE_O) $(FIND_O) $(INSERT_O) \
	   $(INT_KEY_O) $(PRINT_O) $(TREE_O) $(VERIFY_O)


# DEPENDENCIES
# ──────────────────────────────────────────────────────────────────────────────
INT_KEY_O_DEP   := $(INT_KEY_C) $(INT_KEY_H)
INT_KEY_O_GRP   := $(INT_KEY_O)

VERIFY_O_DEP    := $(VERIFY_C) $(VERIFY_H) $(NODE_H) $(COMPARATOR_H)
VERIFY_O_GRP    := $(VERIFY_O)

PRINT_O_DEP     := $(PRINT_C) $(PRINT_H) $(NODE_H) $(PRINT_TYPES_H) $(MALLOC_H)
PRINT_O_GRP     := $(PRINT_O)

FIND_O_DEP      := $(FIND_C) $(FIND_H) $(NODE_H) $(COMPARATOR_H)
FIND_O_GRP      := $(FIND_O)

ALLOCATOR_O_DEP := $(ALLOCATOR_C) $(ALLOCATOR_H) $(NODE_H) $(JUMP_H) $(MALLOC_H)
ALLOCATOR_O_GRP := $(ALLOCATOR_O)

INSERT_O_DEP    := $(INSERT_C) $(INSERT_H) $(COMPARATOR_H) $(ALLOCATOR_H)
INSERT_O_GRP    := $(INSERT_O) $(ALLOCATOR_O_GRP)

DELETE_O_DEP    := $(DELETE_C) $(DELETE_H) $(COMPARATOR_H) $(ALLOCATOR_H)
DELETE_O_GRP    := $(DELETE_O) $(ALLOCATOR_O_GRP)

TREE_O_DEP      := $(TREE_C) $(TREE_H) $(COMPARATOR_H) $(ALLOCATOR_H) \
	           $(PRINT_TYPES_H) $(INSERT_H) $(DELETE_H) $(FIND_H) \
		   $(VERIFY_H) $(PRINT_H)
TREE_O_GRP      := $(TREE_O) $(INSERT_O_GRP) $(DELETE_O_GRP) $(FIND_O_GRP) \
	           $(VERIFY_O_GRP) $(PRINT_O_GRP)

MAIN_O_DEP      := $(MAIN_C) $(MAIN_H) $(TREE_H) $(INT_KEY_H)
MAIN_O_GRP      := $(MAIN_O) $(TREE_O_GRP) $(INT_KEY_O_GRP)

MAIN_DEP        := $(MAIN_O) $(MAIN_O_GRP)


# MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
all: $(TARGETS)

$(MAIN): $(MAIN_DEP)
	$(LD) $(LD_FLAGS) $^ -o $@

$(MAIN_O): $(MAIN_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(TREE_O): $(TREE_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(DELETE_O): $(DELETE_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(INSERT_O): $(INSERT_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(ALLOCATOR_O): $(ALLOCATOR_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(FIND_O): $(FIND_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(PRINT_O): $(PRINT_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(VERIFY_O): $(VERIFY_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(INT_KEY_O): $(INT_KEY_O_DEP)
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)
