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




# PATH CONFIGURATION
# ══════════════════════════════════════════════════════════════════════════════
# String Utils
# ──────────────────────────────────────────────────────────────────────────────
EMPTY	       :=
SPACE	       := $(EMPTY) $(EMPTY)
TAB	       := $(EMPTY)	$(EMPTY)
TRIM		= $(subst $(SPACE),$(EMPTY),$1)
JOIN	        = $(subst $(SPACE),$2,$(strip $1))
define NEWLINE :=


endef


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
PIC_OBJECT_FILE_PATH     = $(call FILE_PATH,$1,$2_pic$(OBJ_EXT))
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
PIC_OBJECT_PATH      = $(call PIC_OBJECT_FILE_PATH,$(OBJECT_DIR),$1)
BINARY_PATH          = $(call BINARY_FILE_PATH,$(BINARY_DIR),$1)
STATIC_LIBRARY_PATH  = $(call STATIC_LIBRARY_FILE_PATH,$(STATIC_LIBRARY_DIR),$1)
SHARED_LIBRARY_PATH  = $(call SHARED_LIBRARY_FILE_PATH,$(SHARED_LIBRARY_DIR),$1)




# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_BASE_FLAGS	:= -std=gnu99 -march=native
CC_ENV_FLAGS	:= -D__USE_FIXED_PROTOTYPES__ $(SYS_ENV_FLAGS)
CC_INC_FLAGS	:= -I$(HEADER_DIR) -I$(KEY_ACC_DIR)
CC_FLAGS	:= $(CC_BASE_FLAGS) $(CC_ENV_FLAGS) $(CC_INC_FLAGS) -O2 -funroll-loops -c
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




# RED_BLACK MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_HDR := $(call HEADER_PATH,red_black_stack_count)
COMPARATOR_HDR  := $(call HEADER_PATH,red_black_comparator)
JUMP_HDR        := $(call HEADER_PATH,red_black_jump)
MALLOC_HDR      := $(call HEADER_PATH,red_black_malloc)
NODE_HDR        := $(call HEADER_PATH,red_black_node)
LOCK_HDR        := $(call HEADER_PATH,red_black_lock)
PRINT_TYPES_HDR := $(call HEADER_PATH,red_black_print_types)


# red_black_allocator
# ──────────────────────────────────────────────────────────────────────────────
ALLOCATOR_SRC		:= $(call SOURCE_PATH,red_black_allocator)
ALLOCATOR_HDR		:= $(call HEADER_PATH,red_black_allocator)
ALLOCATOR_OBJ		:= $(call OBJECT_PATH,red_black_allocator)
ALLOCATOR_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_allocator)
# ─────────────── target prequisites ───────────────────────────────────────────
ALLOCATOR_OBJ_PREQS	:= $(ALLOCATOR_SRC)				\
			   $(ALLOCATOR_HDR)				\
			   $(NODE_HDR)					\
			   $(JUMP_HDR)					\
			   $(MALLOC_HDR)
ALLOCATOR_OBJ_GROUP	:= $(ALLOCATOR_OBJ)
ALLOCATOR_PIC_OBJ_PREQS	:= $(ALLOCATOR_OBJ_PREQS)
ALLOCATOR_PIC_OBJ_GROUP := $(ALLOCATOR_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(ALLOCATOR_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ)


# red_black_iterator
# ──────────────────────────────────────────────────────────────────────────────
ITERATOR_SRC		:= $(call SOURCE_PATH,red_black_iterator)
ITERATOR_HDR		:= $(call HEADER_PATH,red_black_iterator)
ITERATOR_OBJ		:= $(call OBJECT_PATH,red_black_iterator)
ITERATOR_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_iterator)
# ─────────────── target prequisites ───────────────────────────────────────────
ITERATOR_OBJ_PREQS	:= $(ITERATOR_SRC)				\
			   $(ITERATOR_HDR)				\
			   $(NODE_HDR)					\
			   $(STACK_COUNT_HDR)
ITERATOR_OBJ_GROUP	:= $(ITERATOR_OBJ)
ITERATOR_PIC_OBJ_PREQS	:= $(ITERATOR_OBJ_PREQS)
ITERATOR_PIC_OBJ_GROUP	:= $(ITERATOR_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(ITERATOR_OBJ)				\
			   $(ITERATOR_PIC_OBJ)

# red_black_correct
# ──────────────────────────────────────────────────────────────────────────────
CORRECT_SRC		:= $(call SOURCE_PATH,red_black_correct)
CORRECT_HDR		:= $(call HEADER_PATH,red_black_correct)
CORRECT_OBJ		:= $(call OBJECT_PATH,red_black_correct)
CORRECT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_correct)
# ─────────────── target prequisites ───────────────────────────────────────────
CORRECT_OBJ_PREQS	:= $(CORRECT_SRC)				\
			   $(CORRECT_HDR)				\
			   $(NODE_HDR)					\
			   $(JUMP_HDR)
CORRECT_OBJ_GROUP	:= $(CORRECT_OBJ)
CORRECT_PIC_OBJ_PREQS	:= $(CORRECT_OBJ_PREQS)
CORRECT_PIC_OBJ_GROUP	:= $(CORRECT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CORRECT_OBJ)				\
			   $(CORRECT_PIC_OBJ)


# red_black_insert
# ──────────────────────────────────────────────────────────────────────────────
INSERT_SRC		:= $(call SOURCE_PATH,red_black_insert)
INSERT_HDR		:= $(call HEADER_PATH,red_black_insert)
INSERT_OBJ		:= $(call OBJECT_PATH,red_black_insert)
INSERT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_insert)
# ─────────────── target prequisites ───────────────────────────────────────────
INSERT_OBJ_PREQS	:= $(INSERT_SRC)				\
			   $(INSERT_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(ALLOCATOR_HDR)				\
			   $(CORRECT_HDR)
INSERT_OBJ_GROUP	:= $(INSERT_OBJ)				\
			   $(ALLOCATOR_OBJ_GROUP)			\
			   $(CORRECT_OBJ_GROUP)
INSERT_PIC_OBJ_PREQS	:= $(INSERT_OBJ_PREQS)
INSERT_PIC_OBJ_GROUP	:= $(INSERT_PIC_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ_GROUP)			\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(INSERT_OBJ)				\
			   $(INSERT_PIC_OBJ)


# red_black_update
# ──────────────────────────────────────────────────────────────────────────────
UPDATE_SRC		:= $(call SOURCE_PATH,red_black_update)
UPDATE_HDR		:= $(call HEADER_PATH,red_black_update)
UPDATE_OBJ		:= $(call OBJECT_PATH,red_black_update)
UPDATE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_update)
# ─────────────── target prequisites ───────────────────────────────────────────
UPDATE_OBJ_PREQS	:= $(UPDATE_SRC)				\
			   $(UPDATE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(ALLOCATOR_HDR)				\
			   $(CORRECT_HDR)
UPDATE_OBJ_GROUP	:= $(UPDATE_OBJ)				\
			   $(ALLOCATOR_OBJ_GROUP)			\
			   $(CORRECT_OBJ_GROUP)
UPDATE_PIC_OBJ_PREQS	:= $(UPDATE_OBJ_PREQS)
UPDATE_PIC_OBJ_GROUP	:= $(UPDATE_PIC_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ_GROUP)			\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(UPDATE_OBJ)				\
			   $(UPDATE_PIC_OBJ)


# red_black_append
# ──────────────────────────────────────────────────────────────────────────────
APPEND_SRC		:= $(call SOURCE_PATH,red_black_append)
APPEND_HDR		:= $(call HEADER_PATH,red_black_append)
APPEND_OBJ		:= $(call OBJECT_PATH,red_black_append)
APPEND_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_append)
# ─────────────── target prequisites ───────────────────────────────────────────
APPEND_OBJ_PREQS	:= $(APPEND_SRC)				\
			   $(APPEND_HDR)				\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(JUMP_HDR)					\
			   $(CORRECT_HDR)
APPEND_OBJ_GROUP	:= $(APPEND_OBJ)				\
			   $(CORRECT_OBJ_GROUP)
APPEND_PIC_OBJ_PREQS	:= $(APPEND_OBJ_PREQS)
APPEND_PIC_OBJ_GROUP	:= $(APPEND_PIC_OBJ)				\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(APPEND_OBJ)				\
			   $(APPEND_PIC_OBJ)


# red_black_restore
# ──────────────────────────────────────────────────────────────────────────────
RESTORE_SRC		:= $(call SOURCE_PATH,red_black_restore)
RESTORE_HDR		:= $(call HEADER_PATH,red_black_restore)
RESTORE_OBJ		:= $(call OBJECT_PATH,red_black_restore)
RESTORE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_restore)
# ─────────────── target prequisites ───────────────────────────────────────────
RESTORE_OBJ_PREQS	:= $(RESTORE_SRC)				\
			   $(RESTORE_HDR)				\
			   $(NODE_HDR)					\
			   $(JUMP_HDR)					\
			   $(STACK_COUNT_HDR)
RESTORE_OBJ_GROUP	:= $(RESTORE_OBJ)
RESTORE_PIC_OBJ_PREQS	:= $(RESTORE_OBJ_PREQS)
RESTORE_PIC_OBJ_GROUP	:= $(RESTORE_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(RESTORE_OBJ)				\
			   $(RESTORE_PIC_OBJ)


# red_black_delete
# ──────────────────────────────────────────────────────────────────────────────
DELETE_SRC		:= $(call SOURCE_PATH,red_black_delete)
DELETE_HDR		:= $(call HEADER_PATH,red_black_delete)
DELETE_OBJ		:= $(call OBJECT_PATH,red_black_delete)
DELETE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_delete)
# ─────────────── target prequisites ───────────────────────────────────────────
DELETE_OBJ_PREQS	:= $(DELETE_SRC)				\
			   $(DELETE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(ALLOCATOR_HDR)				\
			   $(RESTORE_HDR)
DELETE_OBJ_GROUP	:= $(DELETE_OBJ)				\
			   $(ALLOCATOR_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
DELETE_PIC_OBJ_PREQS	:= $(DELETE_OBJ_PREQS)
DELETE_PIC_OBJ_GROUP	:= $(DELETE_PIC_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ_GROUP)			\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(DELETE_OBJ)				\
			   $(DELETE_PIC_OBJ)


# red_black_remove
# ──────────────────────────────────────────────────────────────────────────────
REMOVE_SRC		:= $(call SOURCE_PATH,red_black_remove)
REMOVE_HDR		:= $(call HEADER_PATH,red_black_remove)
REMOVE_OBJ		:= $(call OBJECT_PATH,red_black_remove)
REMOVE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_remove)
# ─────────────── target prequisites ───────────────────────────────────────────
REMOVE_OBJ_PREQS	:= $(REMOVE_SRC)				\
			   $(REMOVE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(ALLOCATOR_HDR)				\
			   $(RESTORE_HDR)
REMOVE_OBJ_GROUP	:= $(REMOVE_OBJ)				\
			   $(ALLOCATOR_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
REMOVE_PIC_OBJ_PREQS	:= $(REMOVE_OBJ_PREQS)
REMOVE_PIC_OBJ_GROUP	:= $(REMOVE_PIC_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ_GROUP)			\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(REMOVE_OBJ)				\
			   $(REMOVE_PIC_OBJ)


# red_black_find
# ──────────────────────────────────────────────────────────────────────────────
FIND_SRC		:= $(call SOURCE_PATH,red_black_find)
FIND_HDR		:= $(call HEADER_PATH,red_black_find)
FIND_OBJ		:= $(call OBJECT_PATH,red_black_find)
FIND_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_find)
# ─────────────── target prequisites ───────────────────────────────────────────
FIND_OBJ_PREQS		:= $(FIND_SRC)					\
			   $(FIND_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
FIND_OBJ_GROUP		:= $(FIND_OBJ)
FIND_PIC_OBJ_PREQS	:= $(FIND_OBJ_PREQS)
FIND_PIC_OBJ_GROUP	:= $(FIND_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(FIND_OBJ)					\
			   $(FIND_PIC_OBJ)


# red_black_fetch
# ──────────────────────────────────────────────────────────────────────────────
FETCH_SRC		:= $(call SOURCE_PATH,red_black_fetch)
FETCH_HDR		:= $(call HEADER_PATH,red_black_fetch)
FETCH_OBJ		:= $(call OBJECT_PATH,red_black_fetch)
FETCH_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_fetch)
# ─────────────── target prequisites ───────────────────────────────────────────
FETCH_OBJ_PREQS		:= $(FETCH_SRC)					\
			   $(FETCH_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
FETCH_OBJ_GROUP		:= $(FETCH_OBJ)
FETCH_PIC_OBJ_PREQS	:= $(FETCH_OBJ_PREQS)
FETCH_PIC_OBJ_GROUP	:= $(FETCH_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(FETCH_OBJ)					\
			   $(FETCH_PIC_OBJ)


# red_black_count
# ──────────────────────────────────────────────────────────────────────────────
COUNT_SRC		:= $(call SOURCE_PATH,red_black_count)
COUNT_HDR		:= $(call HEADER_PATH,red_black_count)
COUNT_OBJ		:= $(call OBJECT_PATH,red_black_count)
COUNT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_count)
# ─────────────── target prequisites ───────────────────────────────────────────
COUNT_OBJ_PREQS		:= $(COUNT_SRC)					\
			   $(COUNT_HDR)					\
			   $(NODE_HDR)					\
			   $(STACK_COUNT_HDR)
COUNT_OBJ_GROUP		:= $(COUNT_OBJ)
COUNT_PIC_OBJ_PREQS	:= $(COUNT_OBJ_PREQS)
COUNT_PIC_OBJ_GROUP	:= $(COUNT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(COUNT_OBJ)					\
			   $(COUNT_PIC_OBJ)


# red_black_print
# ──────────────────────────────────────────────────────────────────────────────
PRINT_SRC		:= $(call SOURCE_PATH,red_black_print)
PRINT_HDR		:= $(call HEADER_PATH,red_black_print)
PRINT_OBJ		:= $(call OBJECT_PATH,red_black_print)
PRINT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_print)
# ─────────────── target prequisites ───────────────────────────────────────────
PRINT_OBJ_PREQS		:= $(PRINT_SRC)					\
			   $(PRINT_HDR)					\
			   $(NODE_HDR)					\
			   $(PRINT_TYPES_HDR)				\
			   $(MALLOC_HDR)
PRINT_OBJ_GROUP		:= $(PRINT_OBJ)
PRINT_PIC_OBJ_PREQS	:= $(PRINT_OBJ_PREQS)
PRINT_PIC_OBJ_GROUP	:= $(PRINT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(PRINT_OBJ)					\
			   $(PRINT_PIC_OBJ)


# red_black_verify
# ──────────────────────────────────────────────────────────────────────────────
VERIFY_SRC		:= $(call SOURCE_PATH,red_black_verify)
VERIFY_HDR		:= $(call HEADER_PATH,red_black_verify)
VERIFY_OBJ		:= $(call OBJECT_PATH,red_black_verify)
VERIFY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_verify)
# ─────────────── target prequisites ───────────────────────────────────────────
VERIFY_OBJ_PREQS	:= $(VERIFY_SRC)				\
			   $(VERIFY_HDR)				\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
VERIFY_OBJ_GROUP	:= $(VERIFY_OBJ)
VERIFY_PIC_OBJ_PREQS	:= $(VERIFY_OBJ_PREQS)
VERIFY_PIC_OBJ_GROUP	:= $(VERIFY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(VERIFY_OBJ)				\
			   $(VERIFY_PIC_OBJ)


# red_black_concat
# ──────────────────────────────────────────────────────────────────────────────
CONCAT_SRC		:= $(call SOURCE_PATH,red_black_concat)
CONCAT_HDR		:= $(call HEADER_PATH,red_black_concat)
CONCAT_OBJ		:= $(call OBJECT_PATH,red_black_concat)
CONCAT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_concat)
# ─────────────── target prequisites ───────────────────────────────────────────
CONCAT_OBJ_PREQS	:= $(CONCAT_SRC)				\
			   $(CONCAT_HDR)				\
			   $(NODE_HDR)
CONCAT_OBJ_GROUP	:= $(CONCAT_OBJ)
CONCAT_PIC_OBJ_PREQS	:= $(CONCAT_OBJ_PREQS)
CONCAT_PIC_OBJ_GROUP	:= $(CONCAT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CONCAT_OBJ)				\
			   $(CONCAT_PIC_OBJ)


# red_black_tree
# ──────────────────────────────────────────────────────────────────────────────
TREE_SRC	:= $(call SOURCE_PATH,red_black_tree)
TREE_HDR	:= $(call HEADER_PATH,red_black_tree)
TREE_OBJ	:= $(call OBJECT_PATH,red_black_tree)
TREE_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_tree)
TREE_ST_LIB	:= $(call STATIC_LIBRARY_PATH,red_black_tree)
TREE_SH_LIB	:= $(call SHARED_LIBRARY_PATH,red_black_tree)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_OBJ_PREQS		:= $(TREE_SRC)					\
			   $(TREE_HDR)					\
		   	   $(COMPARATOR_HDR)				\
		   	   $(ALLOCATOR_HDR)				\
		   	   $(ITERATOR_HDR)				\
		   	   $(PRINT_TYPES_HDR)
TREE_OBJ_GROUP		:= $(TREE_OBJ)					\
			   $(ALLOCATOR_OBJ_GROUP)			\
			   $(ITERATOR_OBJ_GROUP)			\
			   $(INSERT_OBJ_GROUP)				\
			   $(UPDATE_OBJ_GROUP)				\
			   $(DELETE_OBJ_GROUP)				\
			   $(REMOVE_OBJ_GROUP)				\
			   $(FIND_OBJ_GROUP)				\
			   $(FETCH_OBJ_GROUP)				\
			   $(COUNT_OBJ_GROUP)				\
			   $(PRINT_OBJ_GROUP)				\
			   $(VERIFY_OBJ_GROUP)
TREE_PIC_OBJ_PREQS	:= $(TREE_OBJ_PREQS)
TREE_PIC_OBJ_GROUP	:= $(TREE_PIC_OBJ)				\
			   $(ALLOCATOR_PIC_OBJ_GROUP)			\
			   $(ITERATOR_PIC_OBJ_GROUP)			\
			   $(INSERT_PIC_OBJ_GROUP)			\
			   $(UPDATE_PIC_OBJ_GROUP)			\
			   $(DELETE_PIC_OBJ_GROUP)			\
			   $(REMOVE_PIC_OBJ_GROUP)			\
			   $(FIND_PIC_OBJ_GROUP)			\
			   $(FETCH_PIC_OBJ_GROUP)			\
			   $(COUNT_PIC_OBJ_GROUP)			\
			   $(PRINT_PIC_OBJ_GROUP)			\
			   $(VERIFY_PIC_OBJ_GROUP)
TREE_ST_LIB_PREQS	:= $(TREE_OBJ_GROUP)
TREE_SH_LIB_PREQS	:= $(TREE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREE_OBJ)					\
			   $(TREE_PIC_OBJ)				\
			   $(TREE_ST_LIB)				\
			   $(TREE_SH_LIB)



# KEY_ACCESSOR MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# int_key
# ──────────────────────────────────────────────────────────────────────────────
INT_KEY_SRC		:= $(call KEY_ACC_SOURCE_PATH,int_key)
INT_KEY_HDR		:= $(call KEY_ACC_HEADER_PATH,int_key)
INT_KEY_OBJ		:= $(call OBJECT_PATH,int_key)
# ─────────────── target prequisites ───────────────────────────────────────────
INT_KEY_OBJ_PREQS	:= $(INT_KEY_SRC)				\
			   $(INT_KEY_HDR)
INT_KEY_OBJ_GROUP	:= $(INT_KEY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(INT_KEY_OBJ)


# str_key
# ──────────────────────────────────────────────────────────────────────────────
STR_KEY_SRC		:= $(call KEY_ACC_SOURCE_PATH,str_key)
STR_KEY_HDR		:= $(call KEY_ACC_HEADER_PATH,str_key)
STR_KEY_OBJ		:= $(call OBJECT_PATH,str_key)
# ─────────────── target prequisites ───────────────────────────────────────────
STR_KEY_OBJ_PREQS	:= $(STR_KEY_SRC)				\
			   $(STR_KEY_HDR)
STR_KEY_OBJ_GROUP	:= $(STR_KEY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(STR_KEY_OBJ)



# TEST MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# red_black_concat_test
# ──────────────────────────────────────────────────────────────────────────────
CONCAT_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_concat_test)
CONCAT_TEST_OBJ		:= $(call OBJECT_PATH,red_black_concat_test)
CONCAT_TEST_BIN		:= $(call BINARY_PATH,red_black_concat_test)
# ─────────────── target prequisites ───────────────────────────────────────────
CONCAT_TEST_OBJ_PREQS	:= $(CONCAT_TEST_SRC)				\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)				\
			   $(CONCAT_HDR)
CONCAT_TEST_BIN_PREQS	:= $(CONCAT_TEST_OBJ)				\
			   $(CONCAT_OBJ_GROUP)				\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CONCAT_TEST_OBJ)				\
			   $(CONCAT_TEST_BIN)


# red_black_append_test
# ──────────────────────────────────────────────────────────────────────────────
APPEND_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_append_test)
APPEND_TEST_OBJ		:= $(call OBJECT_PATH,red_black_append_test)
APPEND_TEST_BIN		:= $(call BINARY_PATH,red_black_append_test)
# ─────────────── target prequisites ───────────────────────────────────────────
APPEND_TEST_OBJ_PREQS	:= $(APPEND_TEST_SRC)				\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)				\
			   $(APPEND_HDR)
APPEND_TEST_BIN_PREQS	:= $(APPEND_TEST_OBJ)				\
			   $(APPEND_OBJ_GROUP)				\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(APPEND_TEST_OBJ)				\
			   $(APPEND_TEST_BIN)

# red_black_tree_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_tree_test)
TREE_TEST_HDR		:= $(call TEST_HEADER_PATH,red_black_tree_test)
TREE_TEST_OBJ		:= $(call OBJECT_PATH,red_black_tree_test)
TREE_TEST_BIN		:= $(call BINARY_PATH,red_black_tree_test)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_TEST_OBJ_PREQS	:= $(TREE_TEST_SRC)				\
			   $(TREE_TEST_HDR)				\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)
TREE_TEST_BIN_PREQS	:= $(TREE_TEST_OBJ)				\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREE_TEST_OBJ)				\
			   $(TREE_TEST_BIN)



# EXAMPLE MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
EXAMPLES_IO_HDR		:= $(call EXAMPLES_HEADER_PATH,examples_io)


# demo
# ──────────────────────────────────────────────────────────────────────────────
DEMO_SRC		:= $(call EXAMPLES_SOURCE_PATH,demo)
DEMO_OBJ		:= $(call OBJECT_PATH,demo)
DEMO_BIN		:= $(call BINARY_PATH,demo)
# ─────────────── target prequisites ───────────────────────────────────────────
DEMO_OBJ_PREQS		:= $(DEMO_SRC)					\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)				\
			   $(EXAMPLES_IO_HDR)
DEMO_BIN_PREQS		:= $(DEMO_OBJ)					\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(DEMO_OBJ)					\
			   $(DEMO_BIN)


# usort
# ──────────────────────────────────────────────────────────────────────────────
USORT_SRC		:= $(call EXAMPLES_SOURCE_PATH,usort)
USORT_OBJ		:= $(call OBJECT_PATH,usort)
USORT_BIN		:= $(call BINARY_PATH,usort)
# ─────────────── target prequisites ───────────────────────────────────────────
USORT_OBJ_PREQS		:= $(USORT_SRC)					\
			   $(TREE_HDR)					\
			   $(STR_KEY_HDR)
USORT_BIN_PREQS		:= $(USORT_OBJ)					\
			   $(STR_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(USORT_OBJ)					\
			   $(USORT_BIN)


# contacts
# ──────────────────────────────────────────────────────────────────────────────
CONTACTS_SRC		:= $(call EXAMPLES_SOURCE_PATH,contacts)
CONTACTS_OBJ		:= $(call OBJECT_PATH,contacts)
CONTACTS_BIN		:= $(call BINARY_PATH,contacts)
# ─────────────── target prequisites ───────────────────────────────────────────
CONTACTS_OBJ_PREQS	:= $(CONTACTS_SRC)				\
			   $(TREE_HDR)					\
			   $(STR_KEY_HDR)				\
			   $(EXAMPLES_IO_HDR)
CONTACTS_BIN_PREQS	:= $(CONTACTS_OBJ)				\
			   $(STR_KEY_OBJ_GROUP)				\
			   $(TREE_ST_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CONTACTS_OBJ)				\
			   $(CONTACTS_BIN)




# MAKE RULES
# ══════════════════════════════════════════════════════════════════════════════
# Phony targets
# ──────────────────────────────────────────────────────────────────────────────
.PHONY: all clean

all: $(TARGETS)

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)


# EXAMPLE MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(DEMO_BIN): $(DEMO_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(DEMO_OBJ): $(DEMO_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(USORT_BIN): $(USORT_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(USORT_OBJ): $(USORT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(CONTACTS_BIN): $(CONTACTS_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(CONTACTS_OBJ): $(CONTACTS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


# TEST MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(TREE_TEST_BIN): $(TREE_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_TEST_OBJ): $(TREE_TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(CONCAT_TEST_BIN): $(CONCAT_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(CONCAT_TEST_OBJ): $(CONCAT_TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(APPEND_TEST_BIN): $(APPEND_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(APPEND_TEST_OBJ): $(APPEND_TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


# RED_BLACK MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(TREE_ST_LIB): $(TREE_ST_LIB_PREQS)
	$(AR) $(AR_FLAGS) $@ $^
$(TREE_SH_LIB): $(TREE_SH_LIB_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_SH_FLAGS) -o $@
$(TREE_OBJ): $(TREE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(TREE_PIC_OBJ): $(TREE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ALLOCATOR_OBJ): $(ALLOCATOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(ALLOCATOR_PIC_OBJ): $(ALLOCATOR_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ITERATOR_OBJ): $(ITERATOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(ITERATOR_PIC_OBJ): $(ITERATOR_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(INSERT_OBJ): $(INSERT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(INSERT_PIC_OBJ): $(INSERT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(UPDATE_OBJ): $(UPDATE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(UPDATE_PIC_OBJ): $(UPDATE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(APPEND_OBJ): $(APPEND_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(APPEND_PIC_OBJ): $(APPEND_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(CORRECT_OBJ): $(CORRECT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(CORRECT_PIC_OBJ): $(CORRECT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


$(DELETE_OBJ): $(DELETE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(DELETE_PIC_OBJ): $(DELETE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(REMOVE_OBJ): $(REMOVE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(REMOVE_PIC_OBJ): $(REMOVE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(RESTORE_OBJ): $(RESTORE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(RESTORE_PIC_OBJ): $(RESTORE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(FIND_OBJ): $(FIND_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(FIND_PIC_OBJ): $(FIND_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(FETCH_OBJ): $(FETCH_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(FETCH_PIC_OBJ): $(FETCH_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(COUNT_OBJ): $(COUNT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(COUNT_PIC_OBJ): $(COUNT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(PRINT_OBJ): $(PRINT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(PRINT_PIC_OBJ): $(PRINT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(VERIFY_OBJ): $(VERIFY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(VERIFY_PIC_OBJ): $(VERIFY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(CONCAT_OBJ): $(CONCAT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(CONCAT_PIC_OBJ): $(CONCAT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(INT_KEY_OBJ): $(INT_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(STR_KEY_OBJ): $(STR_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
