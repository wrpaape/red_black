# SYSTEM CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# extended from user 'Trevor Robinson''s response to stackoverflow question:
#
# 	http://stackoverflow.com/questions/714100/os-detecting-makefile

ifeq ($(OS),Windows_NT)
        SYSTEM_WINDOWS 	:= T
        SYS_ENV_FLAGS 	:= -DWIN32

        ifeq      (AMD64,$(PROCESSOR_ARCHITECTURE))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifeq (x86,$(PROCESSOR_ARCHITECTURE))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS	+= -DIA_32
        endif

else
        UNAME_S := $(shell uname -s)

        ifeq      ($(UNAME_S),Linux)
                SYSTEM_LINUX 	:= T
                SYS_ENV_FLAGS 	:= -DLINUX

        else ifeq ($(UNAME_S),Darwin)
                SYSTEM_OSX 	:= T
                SYS_ENV_FLAGS 	:= -DOSX
        endif

        UNAME_M := $(shell uname -m)

        ifeq      (x86_64,$(UNAME_M))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifneq (,$(filter %86,$(UNAME_M)))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS 	+= -DIA_32

        else ifneq (,$(filter arm%,$(UNAME_M)))

                ifneq (,$(filter %32%,$(UNAME_M)))
                        ARCH_ARM_32	:= T
                        SYS_ARCH_FLAG	:= -arch AArch32
                        SYS_ENV_FLAGS 	+= -DARM_32
                else
                        ARCH_ARM_64	:= T
                        SYS_ARCH_FLAG	:= -arch AArch64
                        SYS_ENV_FLAGS 	+= -DARM_64
                endif
        endif
endif




# String Utils
# ──────────────────────────────────────────────────────────────────────────────
EMPTY	       :=
SPACE	       := $(EMPTY) $(EMPTY)
TAB	       := $(EMPTY)	$(EMPTY)
TRIM		= $(subst $(SPACE),$(EMPTY),$1)
JOIN	        = $(subst $(SPACE),$2,$(strip $1))
define NEWLINE :=


endef




# PATH CONFIGURATION
# ══════════════════════════════════════════════════════════════════════════════
# Path delimiter, file extensions
# ──────────────────────────────────────────────────────────────────────────────
SRC_EXT := .c
HDR_EXT := .h

ifeq (T,$(SYSTEM_WINDOWS))
        PATH_DELIM	:= \\
        OBJ_EXT         := .obj
        BIN_EXT		:= .exe
        ST_LIB_EXT      := .lib
        SH_LIB_EXT      := .dll

else
        PATH_DELIM	:= /
        OBJ_EXT         := .o
        BIN_EXT		:= $(EMPTY)
        ST_LIB_EXT      := .a

        ifeq (T,$(SYSTEM_OSX))
                SH_LIB_EXT := .dylib
        else
                SH_LIB_EXT := .so
        endif
endif


# General file paths
# ──────────────────────────────────────────────────────────────────────────────
FILE_PATH = $(call JOIN,$(call TRIM,$1) $(call TRIM,$2),$(PATH_DELIM))

SOURCE_FILE_PATH         = $(call FILE_PATH,$1,$2$(SRC_EXT))
HEADER_FILE_PATH         = $(call FILE_PATH,$1,$2$(HDR_EXT))
OBJECT_FILE_PATH         = $(call FILE_PATH,$1,$2$(OBJ_EXT))
BINARY_FILE_PATH         = $(call FILE_PATH,$1,$2$(BIN_EXT))
STATIC_LIBRARY_FILE_PATH = $(call FILE_PATH,$1,$2$(ST_LIB_EXT))
SHARED_LIBRARY_FILE_PATH = $(call FILE_PATH,$1,$2$(SH_LIB_EXT))


# Directories
# ──────────────────────────────────────────────────────────────────────────────
SOURCE_DIR          := src# library source files
HEADER_DIR          := include# library header files
TEST_DIR            := test# test source and header files
EXAMPLES_DIR        := examples# example source and hear files
KEY_ACC_DIR         := key_accessors#common key accessors
OBJECT_DIR          := obj# object files
BINARY_DIR          := bin# binary executable files
STATIC_LIBRARY_DIR  := static# static library files
SHARED_LIBRARY_DIR  := shared# shared library files


# Project file paths
# ──────────────────────────────────────────────────────────────────────────────
SOURCE_PATH          = $(call SOURCE_FILE_PATH,$(SOURCE_DIR),$1)
HEADER_PATH          = $(call HEADER_FILE_PATH,$(HEADER_DIR),$1)
TEST_SOURCE_PATH     = $(call SOURCE_FILE_PATH,$(TEST_DIR),$1)
TEST_HEADER_PATH     = $(call HEADER_FILE_PATH,$(TEST_DIR),$1)
EXAMPLES_SOURCE_PATH = $(call SOURCE_FILE_PATH,$(EXAMPLES_DIR),$1)
EXAMPLES_HEADER_PATH = $(call HEADER_FILE_PATH,$(EXAMPLES_DIR),$1)
KEY_ACC_SOURCE_PATH  = $(call SOURCE_FILE_PATH,$(KEY_ACC_DIR),$1)
KEY_ACC_HEADER_PATH  = $(call HEADER_FILE_PATH,$(KEY_ACC_DIR),$1)
OBJECT_PATH          = $(call OBJECT_FILE_PATH,$(OBJECT_DIR),$1)
BINARY_PATH          = $(call BINARY_FILE_PATH,$(BINARY_DIR),$1)
STATIC_LIBRARY_PATH  = $(call STATIC_LIBRARY_FILE_PATH,$(STATIC_LIBRARY_DIR),$1)
SHARED_LIBRARY_PATH  = $(call SHARED_LIBRARY_FILE_PATH,$(SHARED_LIBRARY_DIR),$1)




# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_ENV_FLAGS	:= -D__USE_FIXED_PROTOTYPES__ $(SYS_ENV_FLAGS)
CC_BASE_FLAGS	:= -std=gnu99 -march=native $(CC_ENV_FLAGS) -I$(HEADER_DIR)
CC_FLAGS	:= -O2 -funroll-loops $(CC_BASE_FLAGS)
CC_PIC_FLAGS	:= -fpic

# Archiver
# ──────────────────────────────────────────────────────────────────────────────
AR       := ar
AR_FLAGS := rcs

# Linker
# ──────────────────────────────────────────────────────────────────────────────
LD_LIBS	:= -lc
ifeq (T,$(SYSTEM_OSX))
        LD		:= ld
        LD_FLAGS	:= -macosx_version_min 10.11.0 -no_pie $(SYS_ARCH_FLAG)
        LD_BIN_FLAGS	:= -no_pie
        LD_SH_FLAGS	:= -dylib
else
        LD		:= gcc
        LD_FLAGS	:= $(EMPTY)
        LD_BIN_FLAGS	:= $(EMPTY)
        LD_SH_FLAGS	:= -shared
endif

# Cleaner
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
# 'clean' command may exceed 8192 character limit for windows
        RM		:= cmd \/C del
        RM_FLAGS	:= /F /Q
        #RM		:= rm
        #RM_FLAGS	:= -rf
else
        RM		:= rm
        RM_FLAGS	:= -rf
endif




# PROJECT MODULES
# ══════════════════════════════════════════════════════════════════════════════
# independent headers
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_HDR := $(call HEADER_PATH,red_black_stack_count)
COMPARATOR_HDR  := $(call HEADER_PATH,red_black_comparator)
JUMP_HDR        := $(call HEADER_PATH,red_black_jump)

# HEADER FILES
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_H := $(INCLUDE_DIR)/red_black_stack_count.h
ALLOCATOR_H   := $(INCLUDE_DIR)/red_black_allocator.h
COMPARATOR_H  := $(INCLUDE_DIR)/red_black_comparator.h
ITERATOR_H    := $(INCLUDE_DIR)/red_black_iterator.h
DELETE_H      := $(INCLUDE_DIR)/red_black_delete.h
FIND_H        := $(INCLUDE_DIR)/red_black_find.h
INSERT_H      := $(INCLUDE_DIR)/red_black_insert.h
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
PRINT_C     := $(SRC_DIR)/red_black_print.c
TREE_C      := $(SRC_DIR)/red_black_tree.c
VERIFY_C    := $(SRC_DIR)/red_black_verify.c
COUNT_C     := $(SRC_DIR)/red_black_count.c


# TEST FILES
# ──────────────────────────────────────────────────────────────────────────────
TEST_H := $(TEST_DIR)/red_black_test.h
TEST_C := $(TEST_DIR)/red_black_test.c


# EXAMPLE FILES
# ──────────────────────────────────────────────────────────────────────────────
USORT_C   := $(EXAMPLES_DIR)/red_black_usort.c
DEMO_C    := $(EXAMPLES_DIR)/red_black_demo.c


# KEY ACCESSOR FILES
# ──────────────────────────────────────────────────────────────────────────────
INT_KEY_H := $(KEY_ACC_DIR)/int_key.h
INT_KEY_C := $(KEY_ACC_DIR)/int_key.c
STR_KEY_H := $(KEY_ACC_DIR)/str_key.h
STR_KEY_C := $(KEY_ACC_DIR)/str_key.c


# OBJECT FILES
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_O := $(OBJ_DIR)/red_black_allocator.o
ITERATOR_O  := $(OBJ_DIR)/red_black_iterator.o
DELETE_O    := $(OBJ_DIR)/red_black_delete.o
FIND_O      := $(OBJ_DIR)/red_black_find.o
INSERT_O    := $(OBJ_DIR)/red_black_insert.o
PRINT_O     := $(OBJ_DIR)/red_black_print.o
COUNT_O     := $(OBJ_DIR)/red_black_count.o
TREE_O      := $(OBJ_DIR)/red_black_tree.o
VERIFY_O    := $(OBJ_DIR)/red_black_verify.o
TEST_O      := $(OBJ_DIR)/red_black_test.o
USORT_O     := $(OBJ_DIR)/red_black_usort.o
DEMO_O      := $(OBJ_DIR)/red_black_demo.o
INT_KEY_O   := $(OBJ_DIR)/red_black_int_key.o
STR_KEY_O   := $(OBJ_DIR)/red_black_str_key.o


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

INT_KEY_O_DEP    := $(INT_KEY_C) $(INT_KEY_H)
INT_KEY_O_GRP    := $(INT_KEY_O)

STR_KEY_O_DEP    := $(STR_KEY_C) $(STR_KEY_H)
STR_KEY_O_GRP    := $(STR_KEY_O)

USORT_O_DEP      := $(USORT_C) $(TREE_H) $(STR_KEY_H)
USORT_DEP        := $(USORT_O) $(TREE_ST) $(STR_KEY_O_GRP)

DEMO_O_DEP       := $(DEMO_C) $(TREE_H) $(INT_KEY_H)
DEMO_DEP         := $(DEMO_O) $(TREE_ST) $(INT_KEY_O_GRP)

TEST_O_DEP       := $(TEST_C) $(TEST_H) $(TREE_H) $(INT_KEY_H)
TEST_DEP         := $(TEST_O) $(TREE_ST) $(INT_KEY_O_GRP)

# Phony Targets
# ──────────────────────────────────────────────────────────────────────────────
.PHONY: all clean


# MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
all: $(TARGETS)

$(DEMO): $(DEMO_DEP)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@

$(USORT): $(USORT_DEP)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@

$(TEST): $(TEST_DEP)
	$(LD) $^ $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@

$(TREE_ST): $(TREE_ST_DEP)
	$(AR) $(AR_FLAGS) $@ $^

$(TREE_SH): $(TREE_SH_DEP)
	$(LD) $^ $(LD_FLAGS) $(LD_SH_FLAGS) -o $@

$(USORT_O): $(USORT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TEST_O): $(TEST_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(DEMO_O): $(DEMO_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TREE_O): $(TREE_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(TREE_PO): $(TREE_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(DELETE_O): $(DELETE_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(DELETE_PO): $(DELETE_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(INSERT_O): $(INSERT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(INSERT_PO): $(INSERT_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ALLOCATOR_O): $(ALLOCATOR_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(ALLOCATOR_PO): $(ALLOCATOR_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ITERATOR_O): $(ITERATOR_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(ITERATOR_PO): $(ITERATOR_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(FIND_O): $(FIND_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(FIND_PO): $(FIND_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(PRINT_O): $(PRINT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(PRINT_PO): $(PRINT_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(VERIFY_O): $(VERIFY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(VERIFY_PO): $(VERIFY_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(COUNT_O): $(COUNT_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(COUNT_PO): $(COUNT_PO_DEP)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(INT_KEY_O): $(INT_KEY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

$(STR_KEY_O): $(STR_KEY_O_DEP)
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)
