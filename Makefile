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
PATH_JOIN = $(call JOIN,$(call TRIM,$1) $(call TRIM,$2),$(PATH_DELIM))

SCRIPT_FILE_PATH	 = $(call PATH_JOIN,$1,$2.rb)
SOURCE_FILE_PATH         = $(call PATH_JOIN,$1,$2$(SRC_EXT))
HEADER_FILE_PATH         = $(call PATH_JOIN,$1,$2$(HDR_EXT))
OBJECT_FILE_PATH         = $(call PATH_JOIN,$1,$2$(OBJ_EXT))
PIC_OBJECT_FILE_PATH     = $(call PATH_JOIN,$1,$2_pic$(OBJ_EXT))
BINARY_FILE_PATH         = $(call PATH_JOIN,$1,$2$(BIN_EXT))
STATIC_LIBRARY_FILE_PATH = $(call PATH_JOIN,$1,lib$2$(ST_LIB_EXT))
SHARED_LIBRARY_FILE_PATH = $(call PATH_JOIN,$1,lib$2$(SH_LIB_EXT))


# Directories
# ──────────────────────────────────────────────────────────────────────────────
SOURCE_DIR             := src# library source files
TREE_SOURCE_DIR        := $(call PATH_JOIN,$(SOURCE_DIR),red_black_tree)
HMAP_SOURCE_DIR        := $(call PATH_JOIN,$(SOURCE_DIR),red_black_hmap)
COMMON_SOURCE_DIR      := $(call PATH_JOIN,$(SOURCE_DIR),red_black_common)
HEADER_DIR             := include# library header files
COMMON_HEADER_DIR      := $(call PATH_JOIN,$(HEADER_DIR),red_black_common)
TREE_HEADER_DIR        := $(call PATH_JOIN,$(HEADER_DIR),red_black_tree)
HMAP_HEADER_DIR        := $(call PATH_JOIN,$(HEADER_DIR),red_black_hmap)
TEST_DIR               := test# test source and header files
EXAMPLES_DIR           := examples# example source and hear files
KEY_ACC_DIR            := key_accessors#common key accessors
OBJECT_DIR             := obj# object files
BINARY_DIR             := bin# binary executable files
STATIC_LIBRARY_DIR     := static# static library files
SHARED_LIBRARY_DIR     := shared# shared library files
UNITY_DIR	       := $(call PATH_JOIN,$(TEST_DIR),unity)
UNITY_SOURCE_DIR       := $(call PATH_JOIN,$(UNITY_DIR),src)
UNITY_HEADER_DIR       := $(UNITY_SOURCE_DIR)
UNITY_OBJECT_DIR       := $(call PATH_JOIN,$(UNITY_DIR),obj)
UNITY_AUTO_DIR         := $(call PATH_JOIN,$(UNITY_DIR),auto)
TEST_RUNNER_SOURCE_DIR := $(call PATH_JOIN,$(TEST_DIR),test_runners)


# Project file paths
# ──────────────────────────────────────────────────────────────────────────────
SOURCE_PATH             = $(call SOURCE_FILE_PATH,$(SOURCE_DIR),$1)
TREE_SOURCE_PATH        = $(call SOURCE_FILE_PATH,$(TREE_SOURCE_DIR),$1)
HMAP_SOURCE_PATH        = $(call SOURCE_FILE_PATH,$(HMAP_SOURCE_DIR),$1)
COMMON_SOURCE_PATH      = $(call SOURCE_FILE_PATH,$(COMMON_SOURCE_DIR),$1)
HEADER_PATH             = $(call HEADER_FILE_PATH,$(HEADER_DIR),$1)
TREE_HEADER_PATH        = $(call HEADER_FILE_PATH,$(TREE_HEADER_DIR),$1)
HMAP_HEADER_PATH        = $(call HEADER_FILE_PATH,$(HMAP_HEADER_DIR),$1)
COMMON_HEADER_PATH      = $(call HEADER_FILE_PATH,$(COMMON_HEADER_DIR),$1)
TEST_SOURCE_PATH        = $(call SOURCE_FILE_PATH,$(TEST_DIR),$1_test)
TEST_HEADER_PATH        = $(call HEADER_FILE_PATH,$(TEST_DIR),$1_test)
TEST_RUNNER_SOURCE_PATH = $(call SOURCE_FILE_PATH,$(TEST_RUNNER_SOURCE_DIR),$1_test_runner)
UNITY_SOURCE_PATH	= $(call SOURCE_FILE_PATH,$(UNITY_SOURCE_DIR),$1)
UNITY_HEADER_PATH	= $(call HEADER_FILE_PATH,$(UNITY_HEADER_DIR),$1)
EXAMPLES_SOURCE_PATH 	= $(call SOURCE_FILE_PATH,$(EXAMPLES_DIR),$1)
EXAMPLES_HEADER_PATH 	= $(call HEADER_FILE_PATH,$(EXAMPLES_DIR),$1)
KEY_ACC_SOURCE_PATH  	= $(call SOURCE_FILE_PATH,$(KEY_ACC_DIR),$1)
KEY_ACC_HEADER_PATH  	= $(call HEADER_FILE_PATH,$(KEY_ACC_DIR),$1)
OBJECT_PATH          	= $(call OBJECT_FILE_PATH,$(OBJECT_DIR),$1)
PIC_OBJECT_PATH      	= $(call PIC_OBJECT_FILE_PATH,$(OBJECT_DIR),$1)
TEST_OBJECT_PATH	= $(call OBJECT_PATH,$1_test)
TEST_RUNNER_OBJECT_PATH	= $(call OBJECT_PATH,$1_test_runner)
UNITY_SCRIPT_PATH	= $(call SCRIPT_FILE_PATH,$(UNITY_AUTO_DIR),$1)
UNITY_OBJECT_PATH 	= $(call OBJECT_FILE_PATH,$(UNITY_OBJECT_DIR),$1)
BINARY_PATH          	= $(call BINARY_FILE_PATH,$(BINARY_DIR),$1)
TEST_BINARY_PATH	= $(call BINARY_PATH,$1_test)
STATIC_LIBRARY_PATH  	= $(call STATIC_LIBRARY_FILE_PATH,$(STATIC_LIBRARY_DIR),$1)
SHARED_LIBRARY_PATH  	= $(call SHARED_LIBRARY_FILE_PATH,$(SHARED_LIBRARY_DIR),$1)




# UTILITY CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_BASE_FLAGS	:= -std=gnu99 -march=native -Wall -Wno-unused-result
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

ifeq (T,$(SYSTEM_WINDOWS))
        THREADS_LIB := -lKernel32
else
        THREADS_LIB := -lpthread
endif

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

# Ruby
# ──────────────────────────────────────────────────────────────────────────────
RUBY		:= ruby
RUBY_FLAGS	:= $(EMPTY)




## UNITY TEST FRAMEWORK
# ══════════════════════════════════════════════════════════════════════════════
UNITY_SRC 		:= $(call UNITY_SOURCE_PATH,unity)
UNITY_HDR 		:= $(call UNITY_HEADER_PATH,unity)
UNITY_CONFIG_HDR 	:= $(call UNITY_HEADER_PATH,unity_config)
UNITY_INTERNALS_HDR 	:= $(call UNITY_HEADER_PATH,unity_internals)
UNITY_OBJ 		:= $(call UNITY_OBJECT_PATH,unity)
UNITY_GEN_TRNR_SCRIPT	:= $(call UNITY_SCRIPT_PATH,generate_test_runner)
UNITY_ENV_FLAGS 	:= -DUNITY_INCLUDE_CONFIG_H
UNITY_CC_FLAGS		:= $(CC_FLAGS) $(UNITY_ENV_FLAGS) -I$(UNITY_HEADER_DIR) -I$(TEST_DIR)
# ─────────────── run all tests ────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
TEST_FILES_GLOB		:= $(call BINARY_FILE_PATH,$(BINARY_DIR),*_test)
RUN_TESTS		:= FORFILES /M $(TEST_FILES_GLOB) /C "cmd /c @file"

else
RUN_TESTS_BORDER	:= \n================================================================================
RUN_TESTS		 = for test in $^; do echo "\n$(RUN_TESTS_BORDER)\nRUNNING TEST: $$test$(RUN_TESTS_BORDER)" && ./$$test; done
endif
# ─────────────── target prequisites ───────────────────────────────────────────
UNITY_OBJ_PREQS		:= $(UNITY_SRC)					\
			   $(UNITY_HDR)					\
			   $(UNITY_INTERNALS_HDR)			\
			   $(UNITY_CONFIG_HDR)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(UNITY_OBJ)



# RED_BLACK_COMMON MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_HDR 	  := $(call COMMON_HEADER_PATH,red_black_stack_count)
MALLOC_HDR      	  := $(call COMMON_HEADER_PATH,red_black_malloc)
JUMP_HDR        	  := $(call COMMON_HEADER_PATH,red_black_jump)



# RED_BLACK_TREE MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
COMPARATOR_HDR  	  := $(call TREE_HEADER_PATH,red_black_comparator)
NODE_HDR        	  := $(call TREE_HEADER_PATH,red_black_node)
PRINT_TYPES_HDR 	  := $(call TREE_HEADER_PATH,red_black_print_types)


# red_black_node_factory
# ──────────────────────────────────────────────────────────────────────────────
NODE_FACTORY_SRC		:= $(call TREE_SOURCE_PATH,red_black_node_factory)
NODE_FACTORY_HDR		:= $(call TREE_HEADER_PATH,red_black_node_factory)
NODE_FACTORY_OBJ		:= $(call OBJECT_PATH,red_black_node_factory)
NODE_FACTORY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_node_factory)
# ─────────────── target prequisites ───────────────────────────────────────────
NODE_FACTORY_OBJ_PREQS		:= $(NODE_FACTORY_SRC)			\
			   	   $(NODE_FACTORY_HDR)			\
			   	   $(JUMP_HDR)				\
			   	   $(NODE_HDR)				\
			   	   $(MALLOC_HDR)
NODE_FACTORY_OBJ_GROUP		:= $(NODE_FACTORY_OBJ)
NODE_FACTORY_PIC_OBJ_PREQS	:= $(NODE_FACTORY_OBJ_PREQS)
NODE_FACTORY_PIC_OBJ_GROUP 	:= $(NODE_FACTORY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(NODE_FACTORY_OBJ)			\
			   	   $(NODE_FACTORY_PIC_OBJ)


# red_black_itor
# ──────────────────────────────────────────────────────────────────────────────
ITOR_SRC		:= $(call TREE_SOURCE_PATH,red_black_itor)
ITOR_HDR		:= $(call TREE_HEADER_PATH,red_black_itor)
ITOR_OBJ		:= $(call OBJECT_PATH,red_black_itor)
ITOR_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_itor)
# ─────────────── target prequisites ───────────────────────────────────────────
ITOR_OBJ_PREQS		:= $(ITOR_SRC)					\
			   $(ITOR_HDR)					\
			   $(NODE_HDR)					\
			   $(STACK_COUNT_HDR)
ITOR_OBJ_GROUP		:= $(ITOR_OBJ)
ITOR_PIC_OBJ_PREQS	:= $(ITOR_OBJ_PREQS)
ITOR_PIC_OBJ_GROUP	:= $(ITOR_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(ITOR_OBJ)					\
			   $(ITOR_PIC_OBJ)


# red_black_correct
# ──────────────────────────────────────────────────────────────────────────────
CORRECT_SRC		:= $(call TREE_SOURCE_PATH,red_black_correct)
CORRECT_HDR		:= $(call TREE_HEADER_PATH,red_black_correct)
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
INSERT_SRC		:= $(call TREE_SOURCE_PATH,red_black_insert)
INSERT_HDR		:= $(call TREE_HEADER_PATH,red_black_insert)
INSERT_OBJ		:= $(call OBJECT_PATH,red_black_insert)
INSERT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_insert)
# ─────────────── target prequisites ───────────────────────────────────────────
INSERT_OBJ_PREQS	:= $(INSERT_SRC)				\
			   $(INSERT_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(CORRECT_HDR)
INSERT_OBJ_GROUP	:= $(INSERT_OBJ)				\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(CORRECT_OBJ_GROUP)
INSERT_PIC_OBJ_PREQS	:= $(INSERT_OBJ_PREQS)
INSERT_PIC_OBJ_GROUP	:= $(INSERT_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(INSERT_OBJ)				\
			   $(INSERT_PIC_OBJ)


# red_black_put
# ──────────────────────────────────────────────────────────────────────────────
PUT_SRC			:= $(call TREE_SOURCE_PATH,red_black_put)
PUT_HDR			:= $(call TREE_HEADER_PATH,red_black_put)
PUT_OBJ			:= $(call OBJECT_PATH,red_black_put)
PUT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_put)
# ─────────────── target prequisites ───────────────────────────────────────────
PUT_OBJ_PREQS		:= $(PUT_SRC)					\
			   $(PUT_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(CORRECT_HDR)
PUT_OBJ_GROUP		:= $(PUT_OBJ)					\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(CORRECT_OBJ_GROUP)
PUT_PIC_OBJ_PREQS	:= $(PUT_OBJ_PREQS)
PUT_PIC_OBJ_GROUP	:= $(PUT_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(PUT_OBJ)					\
			   $(PUT_PIC_OBJ)


# red_black_update
# ──────────────────────────────────────────────────────────────────────────────
UPDATE_SRC		:= $(call TREE_SOURCE_PATH,red_black_update)
UPDATE_HDR		:= $(call TREE_HEADER_PATH,red_black_update)
UPDATE_OBJ		:= $(call OBJECT_PATH,red_black_update)
UPDATE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_update)
# ─────────────── target prequisites ───────────────────────────────────────────
UPDATE_OBJ_PREQS	:= $(UPDATE_SRC)				\
			   $(UPDATE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(CORRECT_HDR)
UPDATE_OBJ_GROUP	:= $(UPDATE_OBJ)				\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(CORRECT_OBJ_GROUP)
UPDATE_PIC_OBJ_PREQS	:= $(UPDATE_OBJ_PREQS)
UPDATE_PIC_OBJ_GROUP	:= $(UPDATE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(UPDATE_OBJ)				\
			   $(UPDATE_PIC_OBJ)


# red_black_add
# ──────────────────────────────────────────────────────────────────────────────
ADD_SRC			:= $(call TREE_SOURCE_PATH,red_black_add)
ADD_HDR			:= $(call TREE_HEADER_PATH,red_black_add)
ADD_OBJ			:= $(call OBJECT_PATH,red_black_add)
ADD_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_add)
# ─────────────── target prequisites ───────────────────────────────────────────
ADD_OBJ_PREQS		:= $(ADD_SRC)					\
			   $(ADD_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(JUMP_HDR)					\
			   $(CORRECT_HDR)
ADD_OBJ_GROUP		:= $(ADD_OBJ)					\
			   $(CORRECT_OBJ_GROUP)
ADD_PIC_OBJ_PREQS	:= $(ADD_OBJ_PREQS)
ADD_PIC_OBJ_GROUP	:= $(ADD_PIC_OBJ)				\
			   $(CORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(ADD_OBJ)					\
			   $(ADD_PIC_OBJ)


# red_black_restore
# ──────────────────────────────────────────────────────────────────────────────
RESTORE_SRC		:= $(call TREE_SOURCE_PATH,red_black_restore)
RESTORE_HDR		:= $(call TREE_HEADER_PATH,red_black_restore)
RESTORE_OBJ		:= $(call OBJECT_PATH,red_black_restore)
RESTORE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_restore)
# ─────────────── target prequisites ───────────────────────────────────────────
RESTORE_OBJ_PREQS	:= $(RESTORE_SRC)				\
			   $(RESTORE_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(JUMP_HDR)					\
			   $(STACK_COUNT_HDR)
RESTORE_OBJ_GROUP	:= $(RESTORE_OBJ)				\
			   $(NODE_FACTORY_OBJ_GROUP)
RESTORE_PIC_OBJ_PREQS	:= $(RESTORE_OBJ_PREQS)
RESTORE_PIC_OBJ_GROUP	:= $(RESTORE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(RESTORE_OBJ)				\
			   $(RESTORE_PIC_OBJ)


# red_black_delete
# ──────────────────────────────────────────────────────────────────────────────
DELETE_SRC		:= $(call TREE_SOURCE_PATH,red_black_delete)
DELETE_HDR		:= $(call TREE_HEADER_PATH,red_black_delete)
DELETE_OBJ		:= $(call OBJECT_PATH,red_black_delete)
DELETE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_delete)
# ─────────────── target prequisites ───────────────────────────────────────────
DELETE_OBJ_PREQS	:= $(DELETE_SRC)				\
			   $(DELETE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(RESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
DELETE_OBJ_GROUP	:= $(DELETE_OBJ)				\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
DELETE_PIC_OBJ_PREQS	:= $(DELETE_OBJ_PREQS)
DELETE_PIC_OBJ_GROUP	:= $(DELETE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(DELETE_OBJ)				\
			   $(DELETE_PIC_OBJ)


# red_black_remove
# ──────────────────────────────────────────────────────────────────────────────
REMOVE_SRC		:= $(call TREE_SOURCE_PATH,red_black_remove)
REMOVE_HDR		:= $(call TREE_HEADER_PATH,red_black_remove)
REMOVE_OBJ		:= $(call OBJECT_PATH,red_black_remove)
REMOVE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_remove)
# ─────────────── target prequisites ───────────────────────────────────────────
REMOVE_OBJ_PREQS	:= $(REMOVE_SRC)				\
			   $(REMOVE_HDR)				\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(RESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
REMOVE_OBJ_GROUP	:= $(REMOVE_OBJ)				\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
REMOVE_PIC_OBJ_PREQS	:= $(REMOVE_OBJ_PREQS)
REMOVE_PIC_OBJ_GROUP	:= $(REMOVE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(REMOVE_OBJ)				\
			   $(REMOVE_PIC_OBJ)


# red_black_drop
# ──────────────────────────────────────────────────────────────────────────────
DROP_SRC		:= $(call TREE_SOURCE_PATH,red_black_drop)
DROP_HDR		:= $(call TREE_HEADER_PATH,red_black_drop)
DROP_OBJ		:= $(call OBJECT_PATH,red_black_drop)
DROP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_drop)
# ─────────────── target prequisites ───────────────────────────────────────────
DROP_OBJ_PREQS		:= $(DROP_SRC)					\
			   $(DROP_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(RESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
DROP_OBJ_GROUP		:= $(DROP_OBJ)					\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
DROP_PIC_OBJ_PREQS	:= $(DROP_OBJ_PREQS)
DROP_PIC_OBJ_GROUP	:= $(DROP_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(DROP_OBJ)					\
			   $(DROP_PIC_OBJ)


# red_black_pluck
# ──────────────────────────────────────────────────────────────────────────────
PLUCK_SRC		:= $(call TREE_SOURCE_PATH,red_black_pluck)
PLUCK_HDR		:= $(call TREE_HEADER_PATH,red_black_pluck)
PLUCK_OBJ		:= $(call OBJECT_PATH,red_black_pluck)
PLUCK_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_pluck)
# ─────────────── target prequisites ───────────────────────────────────────────
PLUCK_OBJ_PREQS		:= $(PLUCK_SRC)					\
			   $(PLUCK_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(NODE_FACTORY_HDR)				\
			   $(RESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
PLUCK_OBJ_GROUP		:= $(PLUCK_OBJ)					\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(RESTORE_OBJ_GROUP)
PLUCK_PIC_OBJ_PREQS	:= $(PLUCK_OBJ_PREQS)
PLUCK_PIC_OBJ_GROUP	:= $(PLUCK_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(RESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(PLUCK_OBJ)					\
			   $(PLUCK_PIC_OBJ)


# red_black_find
# ──────────────────────────────────────────────────────────────────────────────
FIND_SRC		:= $(call TREE_SOURCE_PATH,red_black_find)
FIND_HDR		:= $(call TREE_HEADER_PATH,red_black_find)
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
FETCH_SRC		:= $(call TREE_SOURCE_PATH,red_black_fetch)
FETCH_HDR		:= $(call TREE_HEADER_PATH,red_black_fetch)
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


# red_black_replace
# ──────────────────────────────────────────────────────────────────────────────
REPLACE_SRC		:= $(call TREE_SOURCE_PATH,red_black_replace)
REPLACE_HDR		:= $(call TREE_HEADER_PATH,red_black_replace)
REPLACE_OBJ		:= $(call OBJECT_PATH,red_black_replace)
REPLACE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_replace)
# ─────────────── target prequisites ───────────────────────────────────────────
REPLACE_OBJ_PREQS	:= $(REPLACE_SRC)				\
			   $(REPLACE_HDR)				\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
REPLACE_OBJ_GROUP	:= $(REPLACE_OBJ)
REPLACE_PIC_OBJ_PREQS	:= $(REPLACE_OBJ_PREQS)
REPLACE_PIC_OBJ_GROUP	:= $(REPLACE_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(REPLACE_OBJ)				\
			   $(REPLACE_PIC_OBJ)


# red_black_exchange
# ──────────────────────────────────────────────────────────────────────────────
EXCHANGE_SRC		:= $(call TREE_SOURCE_PATH,red_black_exchange)
EXCHANGE_HDR		:= $(call TREE_HEADER_PATH,red_black_exchange)
EXCHANGE_OBJ		:= $(call OBJECT_PATH,red_black_exchange)
EXCHANGE_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_exchange)
# ─────────────── target prequisites ───────────────────────────────────────────
EXCHANGE_OBJ_PREQS	:= $(EXCHANGE_SRC)				\
			   $(EXCHANGE_HDR)				\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
EXCHANGE_OBJ_GROUP	:= $(EXCHANGE_OBJ)
EXCHANGE_PIC_OBJ_PREQS	:= $(EXCHANGE_OBJ_PREQS)
EXCHANGE_PIC_OBJ_GROUP	:= $(EXCHANGE_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(EXCHANGE_OBJ)				\
			   $(EXCHANGE_PIC_OBJ)


# red_black_get
# ──────────────────────────────────────────────────────────────────────────────
GET_SRC			:= $(call TREE_SOURCE_PATH,red_black_get)
GET_HDR			:= $(call TREE_HEADER_PATH,red_black_get)
GET_OBJ			:= $(call OBJECT_PATH,red_black_get)
GET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_get)
# ─────────────── target prequisites ───────────────────────────────────────────
GET_OBJ_PREQS		:= $(GET_SRC)					\
			   $(GET_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
GET_OBJ_GROUP		:= $(GET_OBJ)
GET_PIC_OBJ_PREQS	:= $(GET_OBJ_PREQS)
GET_PIC_OBJ_GROUP	:= $(GET_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(GET_OBJ)					\
			   $(GET_PIC_OBJ)


# red_black_set
# ──────────────────────────────────────────────────────────────────────────────
SET_SRC			:= $(call TREE_SOURCE_PATH,red_black_set)
SET_HDR			:= $(call TREE_HEADER_PATH,red_black_set)
SET_OBJ			:= $(call OBJECT_PATH,red_black_set)
SET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_set)
# ─────────────── target prequisites ───────────────────────────────────────────
SET_OBJ_PREQS		:= $(SET_SRC)					\
			   $(SET_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
SET_OBJ_GROUP		:= $(SET_OBJ)
SET_PIC_OBJ_PREQS	:= $(SET_OBJ_PREQS)
SET_PIC_OBJ_GROUP	:= $(SET_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(SET_OBJ)					\
			   $(SET_PIC_OBJ)


# red_black_swap
# ──────────────────────────────────────────────────────────────────────────────
SWAP_SRC		:= $(call TREE_SOURCE_PATH,red_black_swap)
SWAP_HDR		:= $(call TREE_HEADER_PATH,red_black_swap)
SWAP_OBJ		:= $(call OBJECT_PATH,red_black_swap)
SWAP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_swap)
# ─────────────── target prequisites ───────────────────────────────────────────
SWAP_OBJ_PREQS		:= $(SWAP_SRC)					\
			   $(SWAP_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
SWAP_OBJ_GROUP		:= $(SWAP_OBJ)
SWAP_PIC_OBJ_PREQS	:= $(SWAP_OBJ_PREQS)
SWAP_PIC_OBJ_GROUP	:= $(SWAP_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(SWAP_OBJ)					\
			   $(SWAP_PIC_OBJ)


# red_black_count
# ──────────────────────────────────────────────────────────────────────────────
COUNT_SRC		:= $(call TREE_SOURCE_PATH,red_black_count)
COUNT_HDR		:= $(call TREE_HEADER_PATH,red_black_count)
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
PRINT_SRC		:= $(call TREE_SOURCE_PATH,red_black_print)
PRINT_HDR		:= $(call TREE_HEADER_PATH,red_black_print)
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


# red_black_treeify
# ──────────────────────────────────────────────────────────────────────────────
TREEIFY_SRC		:= $(call TREE_SOURCE_PATH,red_black_treeify)
TREEIFY_HDR		:= $(call TREE_HEADER_PATH,red_black_treeify)
TREEIFY_OBJ		:= $(call OBJECT_PATH,red_black_treeify)
TREEIFY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_treeify)
# ─────────────── target prequisites ───────────────────────────────────────────
TREEIFY_OBJ_PREQS	:= $(TREEIFY_SRC)				\
			   $(TREEIFY_HDR)				\
			   $(NODE_HDR)
TREEIFY_OBJ_GROUP	:= $(TREEIFY_OBJ)
TREEIFY_PIC_OBJ_PREQS	:= $(TREEIFY_OBJ_PREQS)
TREEIFY_PIC_OBJ_GROUP	:= $(TREEIFY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREEIFY_OBJ)				\
			   $(TREEIFY_PIC_OBJ)


# red_black_verify
# ──────────────────────────────────────────────────────────────────────────────
VERIFY_SRC		:= $(call TREE_SOURCE_PATH,red_black_verify)
VERIFY_HDR		:= $(call TREE_HEADER_PATH,red_black_verify)
VERIFY_OBJ		:= $(call OBJECT_PATH,red_black_verify)
VERIFY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_verify)
# ─────────────── target prequisites ───────────────────────────────────────────
VERIFY_OBJ_PREQS	:= $(VERIFY_SRC)				\
			   $(VERIFY_HDR)				\
			   $(NODE_HDR)					\
			   $(JUMP_HDR)					\
			   $(COMPARATOR_HDR)
VERIFY_OBJ_GROUP	:= $(VERIFY_OBJ)
VERIFY_PIC_OBJ_PREQS	:= $(VERIFY_OBJ_PREQS)
VERIFY_PIC_OBJ_GROUP	:= $(VERIFY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(VERIFY_OBJ)				\
			   $(VERIFY_PIC_OBJ)


# red_black_copy
# ──────────────────────────────────────────────────────────────────────────────
COPY_SRC		:= $(call TREE_SOURCE_PATH,red_black_copy)
COPY_HDR		:= $(call TREE_HEADER_PATH,red_black_copy)
COPY_OBJ		:= $(call OBJECT_PATH,red_black_copy)
COPY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_copy)
# ─────────────── target prequisites ───────────────────────────────────────────
COPY_OBJ_PREQS		:= $(COPY_SRC)					\
			   $(COPY_HDR)					\
			   $(NODE_HDR)
COPY_OBJ_GROUP		:= $(COPY_OBJ)
COPY_PIC_OBJ_PREQS	:= $(COPY_OBJ_PREQS)
COPY_PIC_OBJ_GROUP	:= $(COPY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(COPY_OBJ)					\
			   $(COPY_PIC_OBJ)


# red_black_congruent
# ──────────────────────────────────────────────────────────────────────────────
CONGRUENT_SRC		:= $(call TREE_SOURCE_PATH,red_black_congruent)
CONGRUENT_HDR		:= $(call TREE_HEADER_PATH,red_black_congruent)
CONGRUENT_OBJ		:= $(call OBJECT_PATH,red_black_congruent)
CONGRUENT_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_congruent)
# ─────────────── target prequisites ───────────────────────────────────────────
CONGRUENT_OBJ_PREQS	:= $(CONGRUENT_SRC)				\
			   $(CONGRUENT_HDR)				\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)				\
			   $(JUMP_HDR)
CONGRUENT_OBJ_GROUP	:= $(CONGRUENT_OBJ)
CONGRUENT_PIC_OBJ_PREQS	:= $(CONGRUENT_OBJ_PREQS)
CONGRUENT_PIC_OBJ_GROUP	:= $(CONGRUENT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CONGRUENT_OBJ)				\
			   $(CONGRUENT_PIC_OBJ)


# red_black_tree
# ──────────────────────────────────────────────────────────────────────────────
TREE_SRC		:= $(call SOURCE_PATH,red_black_tree)
TREE_HDR		:= $(call HEADER_PATH,red_black_tree)
TREE_OBJ		:= $(call OBJECT_PATH,red_black_tree)
TREE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_tree)
TREE_ST_LIB		:= $(call STATIC_LIBRARY_PATH,red_black_tree)
TREE_SH_LIB		:= $(call SHARED_LIBRARY_PATH,red_black_tree)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_OBJ_PREQS		:= $(TREE_SRC)					\
			   $(TREE_HDR)					\
		   	   $(COMPARATOR_HDR)				\
		   	   $(NODE_FACTORY_HDR)				\
		   	   $(ITOR_HDR)					\
		   	   $(PRINT_TYPES_HDR)				\
		   	   $(INSERT_HDR)				\
		   	   $(PUT_HDR)					\
		   	   $(UPDATE_HDR)				\
		   	   $(ADD_HDR)					\
		   	   $(DELETE_HDR)				\
		   	   $(REMOVE_HDR)				\
		   	   $(DROP_HDR)					\
		   	   $(PLUCK_HDR)					\
		   	   $(FIND_HDR)					\
		   	   $(FETCH_HDR)					\
		   	   $(REPLACE_HDR)				\
		   	   $(EXCHANGE_HDR)				\
		   	   $(GET_HDR)					\
		   	   $(SET_HDR)					\
		   	   $(SWAP_HDR)					\
		   	   $(COUNT_HDR)					\
		   	   $(COPY_HDR)					\
		   	   $(CONGRUENT_HDR)				\
		   	   $(VERIFY_HDR)				\
		   	   $(TREEIFY_HDR)				\
		   	   $(PRINT_HDR)
TREE_OBJ_GROUP		:= $(TREE_OBJ)					\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(ITOR_OBJ_GROUP)				\
			   $(INSERT_OBJ_GROUP)				\
			   $(PUT_OBJ_GROUP)				\
			   $(UPDATE_OBJ_GROUP)				\
			   $(ADD_OBJ_GROUP)				\
			   $(DELETE_OBJ_GROUP)				\
			   $(REMOVE_OBJ_GROUP)				\
		   	   $(DROP_OBJ_GROUP)				\
		   	   $(PLUCK_OBJ_GROUP)				\
			   $(FIND_OBJ_GROUP)				\
			   $(FETCH_OBJ_GROUP)				\
			   $(REPLACE_OBJ_GROUP)				\
			   $(EXCHANGE_OBJ_GROUP)			\
			   $(GET_OBJ_GROUP)				\
			   $(SET_OBJ_GROUP)				\
			   $(SWAP_OBJ_GROUP)				\
			   $(COUNT_OBJ_GROUP)				\
		   	   $(COPY_OBJ_GROUP)				\
			   $(CONGRUENT_OBJ_GROUP)			\
			   $(VERIFY_OBJ_GROUP)				\
		   	   $(TREEIFY_OBJ_GROUP)				\
			   $(PRINT_OBJ_GROUP)
TREE_PIC_OBJ_PREQS	:= $(TREE_OBJ_PREQS)
TREE_PIC_OBJ_GROUP	:= $(TREE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(ITOR_PIC_OBJ_GROUP)			\
			   $(INSERT_PIC_OBJ_GROUP)			\
			   $(PUT_PIC_OBJ_GROUP)				\
			   $(UPDATE_PIC_OBJ_GROUP)			\
			   $(ADD_PIC_OBJ_GROUP)				\
			   $(DELETE_PIC_OBJ_GROUP)			\
			   $(REMOVE_PIC_OBJ_GROUP)			\
		   	   $(DROP_PIC_OBJ_GROUP)			\
		   	   $(PLUCK_PIC_OBJ_GROUP)			\
			   $(FIND_PIC_OBJ_GROUP)			\
			   $(FETCH_PIC_OBJ_GROUP)			\
			   $(REPLACE_PIC_OBJ_GROUP)			\
			   $(EXCHANGE_PIC_OBJ_GROUP)			\
			   $(GET_PIC_OBJ_GROUP)				\
			   $(SET_PIC_OBJ_GROUP)				\
			   $(SWAP_PIC_OBJ_GROUP)			\
			   $(COUNT_PIC_OBJ_GROUP)			\
		   	   $(COPY_PIC_OBJ_GROUP)			\
			   $(CONGRUENT_PIC_OBJ_GROUP)			\
			   $(VERIFY_PIC_OBJ_GROUP)			\
			   $(VERIFY_PIC_OBJ_GROUP)			\
		   	   $(TREEIFY_PIC_OBJ_GROUP)			\
			   $(PRINT_PIC_OBJ_GROUP)
TREE_ST_LIB_PREQS	:= $(TREE_OBJ_GROUP)
TREE_SH_LIB_PREQS	:= $(TREE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREE_OBJ)					\
			   $(TREE_PIC_OBJ)				\
			   $(TREE_ST_LIB)				\
			   $(TREE_SH_LIB)



# RED_BLACK_HMAP MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
HNODE_HDR       	  := $(call HMAP_HEADER_PATH,red_black_hnode)


# red_black_hkey
# ──────────────────────────────────────────────────────────────────────────────
HKEY_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hkey)
HKEY_HDR		:= $(call HMAP_HEADER_PATH,red_black_hkey)
HKEY_OBJ		:= $(call OBJECT_PATH,red_black_hkey)
HKEY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hkey)
# ─────────────── target prequisites ───────────────────────────────────────────
HKEY_OBJ_PREQS		:= $(HKEY_SRC)					\
			   $(HKEY_HDR)
HKEY_OBJ_GROUP		:= $(HKEY_OBJ)
HKEY_PIC_OBJ_PREQS	:= $(HKEY_OBJ_PREQS)
HKEY_PIC_OBJ_GROUP	:= $(HKEY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HKEY_OBJ)					\
			   $(HKEY_PIC_OBJ)


# red_black_hnode_factory
# ──────────────────────────────────────────────────────────────────────────────
HNODE_FACTORY_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hnode_factory)
HNODE_FACTORY_HDR		:= $(call HMAP_HEADER_PATH,red_black_hnode_factory)
HNODE_FACTORY_OBJ		:= $(call OBJECT_PATH,red_black_hnode_factory)
HNODE_FACTORY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hnode_factory)
# ─────────────── target prequisites ───────────────────────────────────────────
HNODE_FACTORY_OBJ_PREQS		:= $(HNODE_FACTORY_SRC)			\
			   	   $(HNODE_FACTORY_HDR)			\
			   	   $(JUMP_HDR)				\
			   	   $(HNODE_HDR)				\
			   	   $(MALLOC_HDR)
HNODE_FACTORY_OBJ_GROUP		:= $(HNODE_FACTORY_OBJ)
HNODE_FACTORY_PIC_OBJ_PREQS	:= $(HNODE_FACTORY_OBJ_PREQS)
HNODE_FACTORY_PIC_OBJ_GROUP 	:= $(HNODE_FACTORY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(HNODE_FACTORY_OBJ)			\
			   	   $(HNODE_FACTORY_PIC_OBJ)


# red_black_hitor
# ──────────────────────────────────────────────────────────────────────────────
HITOR_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hitor)
HITOR_HDR		:= $(call HMAP_HEADER_PATH,red_black_hitor)
HITOR_OBJ		:= $(call OBJECT_PATH,red_black_hitor)
HITOR_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hitor)
# ─────────────── target prequisites ───────────────────────────────────────────
HITOR_OBJ_PREQS		:= $(HITOR_SRC)					\
			   $(HITOR_HDR)					\
			   $(HNODE_HDR)					\
			   $(STACK_COUNT_HDR)
HITOR_OBJ_GROUP		:= $(HITOR_OBJ)
HITOR_PIC_OBJ_PREQS	:= $(HITOR_OBJ_PREQS)
HITOR_PIC_OBJ_GROUP	:= $(HITOR_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HITOR_OBJ)					\
			   $(HITOR_PIC_OBJ)


# red_black_hcorrect
# ──────────────────────────────────────────────────────────────────────────────
HCORRECT_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hcorrect)
HCORRECT_HDR		:= $(call HMAP_HEADER_PATH,red_black_hcorrect)
HCORRECT_OBJ		:= $(call OBJECT_PATH,red_black_hcorrect)
HCORRECT_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_hcorrect)
# ─────────────── target prequisites ───────────────────────────────────────────
HCORRECT_OBJ_PREQS	:= $(HCORRECT_SRC)				\
			   $(HCORRECT_HDR)				\
			   $(HNODE_HDR)					\
			   $(JUMP_HDR)
HCORRECT_OBJ_GROUP	:= $(HCORRECT_OBJ)
HCORRECT_PIC_OBJ_PREQS  := $(HCORRECT_OBJ_PREQS)
HCORRECT_PIC_OBJ_GROUP  := $(HCORRECT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HCORRECT_OBJ)				\
			   $(HCORRECT_PIC_OBJ)


# red_black_hinsert
# ──────────────────────────────────────────────────────────────────────────────
HINSERT_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hinsert)
HINSERT_HDR		:= $(call HMAP_HEADER_PATH,red_black_hinsert)
HINSERT_OBJ		:= $(call OBJECT_PATH,red_black_hinsert)
HINSERT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hinsert)
# ─────────────── target prequisites ───────────────────────────────────────────
HINSERT_OBJ_PREQS	:= $(HINSERT_SRC)				\
			   $(HINSERT_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HCORRECT_HDR)
HINSERT_OBJ_GROUP	:= $(HINSERT_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HCORRECT_OBJ_GROUP)
HINSERT_PIC_OBJ_PREQS	:= $(HINSERT_OBJ_PREQS)
HINSERT_PIC_OBJ_GROUP	:= $(HINSERT_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HCORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HINSERT_OBJ)				\
			   $(HINSERT_PIC_OBJ)


# red_black_hput
# ──────────────────────────────────────────────────────────────────────────────
HPUT_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hput)
HPUT_HDR		:= $(call HMAP_HEADER_PATH,red_black_hput)
HPUT_OBJ		:= $(call OBJECT_PATH,red_black_hput)
HPUT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hput)
# ─────────────── target prequisites ───────────────────────────────────────────
HPUT_OBJ_PREQS		:= $(HPUT_SRC)					\
			   $(HPUT_HDR)					\
			   $(HNODE_FACTORY_HDR)				\
			   $(HCORRECT_HDR)
HPUT_OBJ_GROUP		:= $(HPUT_OBJ)					\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HCORRECT_OBJ_GROUP)
HPUT_PIC_OBJ_PREQS	:= $(HPUT_OBJ_PREQS)
HPUT_PIC_OBJ_GROUP	:= $(HPUT_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HCORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HPUT_OBJ)					\
			   $(HPUT_PIC_OBJ)


# red_black_hupdate
# ──────────────────────────────────────────────────────────────────────────────
HUPDATE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hupdate)
HUPDATE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hupdate)
HUPDATE_OBJ		:= $(call OBJECT_PATH,red_black_hupdate)
HUPDATE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hupdate)
# ─────────────── target prequisites ───────────────────────────────────────────
HUPDATE_OBJ_PREQS	:= $(HUPDATE_SRC)				\
			   $(HUPDATE_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HCORRECT_HDR)
HUPDATE_OBJ_GROUP	:= $(HUPDATE_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HCORRECT_OBJ_GROUP)
HUPDATE_PIC_OBJ_PREQS	:= $(HUPDATE_OBJ_PREQS)
HUPDATE_PIC_OBJ_GROUP	:= $(HUPDATE_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HCORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HUPDATE_OBJ)				\
			   $(HUPDATE_PIC_OBJ)


# red_black_hadd
# ──────────────────────────────────────────────────────────────────────────────
HADD_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hadd)
HADD_HDR		:= $(call HMAP_HEADER_PATH,red_black_hadd)
HADD_OBJ		:= $(call OBJECT_PATH,red_black_hadd)
HADD_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hadd)
# ─────────────── target prequisites ───────────────────────────────────────────
HADD_OBJ_PREQS		:= $(HADD_SRC)					\
			   $(HADD_HDR)					\
			   $(HNODE_HDR)					\
			   $(JUMP_HDR)					\
			   $(HCORRECT_HDR)
HADD_OBJ_GROUP		:= $(HADD_OBJ)					\
			   $(HKEY_OBJ_GROUP)				\
			   $(HCORRECT_OBJ_GROUP)
HADD_PIC_OBJ_PREQS	:= $(HADD_OBJ_PREQS)
HADD_PIC_OBJ_GROUP	:= $(HADD_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HCORRECT_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HADD_OBJ)					\
			   $(HADD_PIC_OBJ)


# red_black_hrestore
# ──────────────────────────────────────────────────────────────────────────────
HRESTORE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hrestore)
HRESTORE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hrestore)
HRESTORE_OBJ		:= $(call OBJECT_PATH,red_black_hrestore)
HRESTORE_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_hrestore)
# ─────────────── target prequisites ───────────────────────────────────────────
HRESTORE_OBJ_PREQS	:= $(HRESTORE_SRC)				\
			   $(HRESTORE_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(JUMP_HDR)					\
			   $(STACK_COUNT_HDR)
HRESTORE_OBJ_GROUP	:= $(HRESTORE_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)
HRESTORE_PIC_OBJ_PREQS	:= $(HRESTORE_OBJ_PREQS)
HRESTORE_PIC_OBJ_GROUP	:= $(HRESTORE_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HRESTORE_OBJ)				\
			   $(HRESTORE_PIC_OBJ)


# red_black_hdelete
# ──────────────────────────────────────────────────────────────────────────────
HDELETE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hdelete)
HDELETE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hdelete)
HDELETE_OBJ		:= $(call OBJECT_PATH,red_black_hdelete)
HDELETE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hdelete)
# ─────────────── target prequisites ───────────────────────────────────────────
HDELETE_OBJ_PREQS	:= $(HDELETE_SRC)				\
			   $(HDELETE_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HRESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
HDELETE_OBJ_GROUP	:= $(HDELETE_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HRESTORE_OBJ_GROUP)
HDELETE_PIC_OBJ_PREQS	:= $(HDELETE_OBJ_PREQS)
HDELETE_PIC_OBJ_GROUP	:= $(HDELETE_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HRESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HDELETE_OBJ)				\
			   $(HDELETE_PIC_OBJ)


# red_black_hdelete
# ──────────────────────────────────────────────────────────────────────────────
HDELETE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hdelete)
HDELETE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hdelete)
HDELETE_OBJ		:= $(call OBJECT_PATH,red_black_hdelete)
HDELETE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hdelete)
# ─────────────── target prequisites ───────────────────────────────────────────
HDELETE_OBJ_PREQS	:= $(HDELETE_SRC)				\
			   $(HDELETE_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HRESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
HDELETE_OBJ_GROUP	:= $(HDELETE_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HRESTORE_OBJ_GROUP)
HDELETE_PIC_OBJ_PREQS	:= $(HDELETE_OBJ_PREQS)
HDELETE_PIC_OBJ_GROUP	:= $(HDELETE_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HRESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HDELETE_OBJ)				\
			   $(HDELETE_PIC_OBJ)


# red_black_hremove
# ──────────────────────────────────────────────────────────────────────────────
HREMOVE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hremove)
HREMOVE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hremove)
HREMOVE_OBJ		:= $(call OBJECT_PATH,red_black_hremove)
HREMOVE_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hremove)
# ─────────────── target prequisites ───────────────────────────────────────────
HREMOVE_OBJ_PREQS	:= $(HREMOVE_SRC)				\
			   $(HREMOVE_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HRESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
HREMOVE_OBJ_GROUP	:= $(HREMOVE_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HRESTORE_OBJ_GROUP)
HREMOVE_PIC_OBJ_PREQS	:= $(HREMOVE_OBJ_PREQS)
HREMOVE_PIC_OBJ_GROUP	:= $(HREMOVE_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HRESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HREMOVE_OBJ)				\
			   $(HREMOVE_PIC_OBJ)


# red_black_hdrop
# ──────────────────────────────────────────────────────────────────────────────
HDROP_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hdrop)
HDROP_HDR		:= $(call HMAP_HEADER_PATH,red_black_hdrop)
HDROP_OBJ		:= $(call OBJECT_PATH,red_black_hdrop)
HDROP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hdrop)
# ─────────────── target prequisites ───────────────────────────────────────────
HDROP_OBJ_PREQS		:= $(HDROP_SRC)					\
			   $(HDROP_HDR)					\
			   $(HNODE_FACTORY_HDR)				\
			   $(HRESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
HDROP_OBJ_GROUP		:= $(HDROP_OBJ)					\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HRESTORE_OBJ_GROUP)
HDROP_PIC_OBJ_PREQS	:= $(HDROP_OBJ_PREQS)
HDROP_PIC_OBJ_GROUP	:= $(HDROP_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HRESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HDROP_OBJ)					\
			   $(HDROP_PIC_OBJ)


# red_black_hpluck
# ──────────────────────────────────────────────────────────────────────────────
HPLUCK_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hpluck)
HPLUCK_HDR		:= $(call HMAP_HEADER_PATH,red_black_hpluck)
HPLUCK_OBJ		:= $(call OBJECT_PATH,red_black_hpluck)
HPLUCK_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hpluck)
# ─────────────── target prequisites ───────────────────────────────────────────
HPLUCK_OBJ_PREQS	:= $(HPLUCK_SRC)				\
			   $(HPLUCK_HDR)				\
			   $(HNODE_FACTORY_HDR)				\
			   $(HRESTORE_HDR)				\
			   $(STACK_COUNT_HDR)
HPLUCK_OBJ_GROUP	:= $(HPLUCK_OBJ)				\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HKEY_OBJ_GROUP)				\
			   $(HRESTORE_OBJ_GROUP)
HPLUCK_PIC_OBJ_PREQS	:= $(HPLUCK_OBJ_PREQS)
HPLUCK_PIC_OBJ_GROUP	:= $(HPLUCK_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HKEY_PIC_OBJ_GROUP)			\
			   $(HRESTORE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HPLUCK_OBJ)				\
			   $(HPLUCK_PIC_OBJ)


# red_black_hfind
# ──────────────────────────────────────────────────────────────────────────────
HFIND_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hfind)
HFIND_HDR		:= $(call HMAP_HEADER_PATH,red_black_hfind)
HFIND_OBJ		:= $(call OBJECT_PATH,red_black_hfind)
HFIND_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hfind)
# ─────────────── target prequisites ───────────────────────────────────────────
HFIND_OBJ_PREQS		:= $(HFIND_SRC)					\
			   $(HFIND_HDR)					\
			   $(HNODE_HDR)
HFIND_OBJ_GROUP		:= $(HFIND_OBJ)					\
			   $(HKEY_OBJ_GROUP)
HFIND_PIC_OBJ_PREQS	:= $(HFIND_OBJ_PREQS)
HFIND_PIC_OBJ_GROUP	:= $(HFIND_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HFIND_OBJ)					\
			   $(HFIND_PIC_OBJ)


# red_black_hfetch
# ──────────────────────────────────────────────────────────────────────────────
HFETCH_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hfetch)
HFETCH_HDR		:= $(call HMAP_HEADER_PATH,red_black_hfetch)
HFETCH_OBJ		:= $(call OBJECT_PATH,red_black_hfetch)
HFETCH_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hfetch)
# ─────────────── target prequisites ───────────────────────────────────────────
HFETCH_OBJ_PREQS	:= $(HFETCH_SRC)				\
			   $(HFETCH_HDR)				\
			   $(HNODE_HDR)
HFETCH_OBJ_GROUP	:= $(HFETCH_OBJ)				\
			   $(HKEY_OBJ_GROUP)
HFETCH_PIC_OBJ_PREQS	:= $(HFETCH_OBJ_PREQS)
HFETCH_PIC_OBJ_GROUP	:= $(HFETCH_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HFETCH_OBJ)				\
			   $(HFETCH_PIC_OBJ)


# red_black_hreplace
# ──────────────────────────────────────────────────────────────────────────────
HREPLACE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hreplace)
HREPLACE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hreplace)
HREPLACE_OBJ		:= $(call OBJECT_PATH,red_black_hreplace)
HREPLACE_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_hreplace)
# ─────────────── target prequisites ───────────────────────────────────────────
HREPLACE_OBJ_PREQS	:= $(HREPLACE_SRC)				\
			   $(HREPLACE_HDR)				\
			   $(HNODE_HDR)
HREPLACE_OBJ_GROUP	:= $(HREPLACE_OBJ)				\
			   $(HKEY_OBJ_GROUP)
HREPLACE_PIC_OBJ_PREQS	:= $(HREPLACE_OBJ_PREQS)
HREPLACE_PIC_OBJ_GROUP	:= $(HREPLACE_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HREPLACE_OBJ)				\
			   $(HREPLACE_PIC_OBJ)


# red_black_hexchange
# ──────────────────────────────────────────────────────────────────────────────
HEXCHANGE_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hexchange)
HEXCHANGE_HDR		:= $(call HMAP_HEADER_PATH,red_black_hexchange)
HEXCHANGE_OBJ		:= $(call OBJECT_PATH,red_black_hexchange)
HEXCHANGE_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_hexchange)
# ─────────────── target prequisites ───────────────────────────────────────────
HEXCHANGE_OBJ_PREQS	:= $(HEXCHANGE_SRC)				\
			   $(HEXCHANGE_HDR)				\
			   $(HNODE_HDR)
HEXCHANGE_OBJ_GROUP	:= $(HEXCHANGE_OBJ)				\
			   $(HKEY_OBJ_GROUP)
HEXCHANGE_PIC_OBJ_PREQS	:= $(HEXCHANGE_OBJ_PREQS)
HEXCHANGE_PIC_OBJ_GROUP	:= $(HEXCHANGE_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HEXCHANGE_OBJ)				\
			   $(HEXCHANGE_PIC_OBJ)


# red_black_hget
# ──────────────────────────────────────────────────────────────────────────────
HGET_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hget)
HGET_HDR		:= $(call HMAP_HEADER_PATH,red_black_hget)
HGET_OBJ		:= $(call OBJECT_PATH,red_black_hget)
HGET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hget)
# ─────────────── target prequisites ───────────────────────────────────────────
HGET_OBJ_PREQS		:= $(HGET_SRC)					\
			   $(HGET_HDR)					\
			   $(HNODE_HDR)
HGET_OBJ_GROUP		:= $(HGET_OBJ)					\
			   $(HKEY_OBJ_GROUP)
HGET_PIC_OBJ_PREQS	:= $(HGET_OBJ_PREQS)
HGET_PIC_OBJ_GROUP	:= $(HGET_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HGET_OBJ)					\
			   $(HGET_PIC_OBJ)


# red_black_hset
# ──────────────────────────────────────────────────────────────────────────────
HSET_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hset)
HSET_HDR		:= $(call HMAP_HEADER_PATH,red_black_hset)
HSET_OBJ		:= $(call OBJECT_PATH,red_black_hset)
HSET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hset)
# ─────────────── target prequisites ───────────────────────────────────────────
HSET_OBJ_PREQS		:= $(HSET_SRC)					\
			   $(HSET_HDR)					\
			   $(HNODE_HDR)
HSET_OBJ_GROUP		:= $(HSET_OBJ)					\
			   $(HKEY_OBJ_GROUP)
HSET_PIC_OBJ_PREQS	:= $(HSET_OBJ_PREQS)
HSET_PIC_OBJ_GROUP	:= $(HSET_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HSET_OBJ)					\
			   $(HSET_PIC_OBJ)

# red_black_hswap
# ──────────────────────────────────────────────────────────────────────────────
HSWAP_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hswap)
HSWAP_HDR		:= $(call HMAP_HEADER_PATH,red_black_hswap)
HSWAP_OBJ		:= $(call OBJECT_PATH,red_black_hswap)
HSWAP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hswap)
# ─────────────── target prequisites ───────────────────────────────────────────
HSWAP_OBJ_PREQS		:= $(HSWAP_SRC)					\
			   $(HSWAP_HDR)					\
			   $(HNODE_HDR)
HSWAP_OBJ_GROUP		:= $(HSWAP_OBJ)					\
			   $(HKEY_OBJ_GROUP)
HSWAP_PIC_OBJ_PREQS	:= $(HSWAP_OBJ_PREQS)
HSWAP_PIC_OBJ_GROUP	:= $(HSWAP_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HSWAP_OBJ)					\
			   $(HSWAP_PIC_OBJ)


# red_black_hverify
# ──────────────────────────────────────────────────────────────────────────────
HVERIFY_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hverify)
HVERIFY_HDR		:= $(call HMAP_HEADER_PATH,red_black_hverify)
HVERIFY_OBJ		:= $(call OBJECT_PATH,red_black_hverify)
HVERIFY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hverify)
# ─────────────── target prequisites ───────────────────────────────────────────
HVERIFY_OBJ_PREQS	:= $(HVERIFY_SRC)				\
			   $(HVERIFY_HDR)				\
			   $(HNODE_HDR)					\
			   $(JUMP_HDR)
HVERIFY_OBJ_GROUP	:= $(HVERIFY_OBJ)				\
			   $(HKEY_OBJ_GROUP)
HVERIFY_PIC_OBJ_PREQS	:= $(HVERIFY_OBJ_PREQS)
HVERIFY_PIC_OBJ_GROUP	:= $(HVERIFY_PIC_OBJ)				\
			   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HVERIFY_OBJ)				\
			   $(HVERIFY_PIC_OBJ)


# red_black_hcopy
# ──────────────────────────────────────────────────────────────────────────────
HCOPY_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hcopy)
HCOPY_HDR		:= $(call HMAP_HEADER_PATH,red_black_hcopy)
HCOPY_OBJ		:= $(call OBJECT_PATH,red_black_hcopy)
HCOPY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hcopy)
# ─────────────── target prequisites ───────────────────────────────────────────
HCOPY_OBJ_PREQS		:= $(HCOPY_SRC)					\
			   $(HCOPY_HDR)					\
			   $(HNODE_HDR)
HCOPY_OBJ_GROUP		:= $(HCOPY_OBJ)
HCOPY_PIC_OBJ_PREQS	:= $(HCOPY_OBJ_PREQS)
HCOPY_PIC_OBJ_GROUP	:= $(HCOPY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HCOPY_OBJ)					\
			   $(HCOPY_PIC_OBJ)


# red_black_hcongruent
# ──────────────────────────────────────────────────────────────────────────────
HCONGRUENT_SRC			:= $(call HMAP_SOURCE_PATH,red_black_hcongruent)
HCONGRUENT_HDR			:= $(call HMAP_HEADER_PATH,red_black_hcongruent)
HCONGRUENT_OBJ			:= $(call OBJECT_PATH,red_black_hcongruent)
HCONGRUENT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hcongruent)
# ─────────────── target prequisites ───────────────────────────────────────────
HCONGRUENT_OBJ_PREQS		:= $(HCONGRUENT_SRC)			\
			   	   $(HCONGRUENT_HDR)			\
			   	   $(HNODE_HDR)				\
			   	   $(JUMP_HDR)
HCONGRUENT_OBJ_GROUP		:= $(HCONGRUENT_OBJ)			\
				   $(HKEY_OBJ_GROUP)
HCONGRUENT_PIC_OBJ_PREQS	:= $(HCONGRUENT_OBJ_PREQS)
HCONGRUENT_PIC_OBJ_GROUP	:= $(HCONGRUENT_PIC_OBJ)		\
				   $(HKEY_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(HCONGRUENT_OBJ)			\
				   $(HCONGRUENT_PIC_OBJ)


# red_black_hconcat
# ──────────────────────────────────────────────────────────────────────────────
HCONCAT_SRC		:= $(call HMAP_SOURCE_PATH,red_black_hconcat)
HCONCAT_HDR		:= $(call HMAP_HEADER_PATH,red_black_hconcat)
HCONCAT_OBJ		:= $(call OBJECT_PATH,red_black_hconcat)
HCONCAT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hconcat)
# ─────────────── target prequisites ───────────────────────────────────────────
HCONCAT_OBJ_PREQS	:= $(HCONCAT_SRC)				\
			   $(HCONCAT_HDR)				\
			   $(HNODE_HDR)					\
			   $(STACK_COUNT_HDR)
HCONCAT_OBJ_GROUP	:= $(HCONCAT_OBJ)
HCONCAT_PIC_OBJ_PREQS	:= $(HCONCAT_OBJ_PREQS)
HCONCAT_PIC_OBJ_GROUP	:= $(HCONCAT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HCONCAT_OBJ)				\
			   $(HCONCAT_PIC_OBJ)


# red_black_hmap
# ──────────────────────────────────────────────────────────────────────────────
HMAP_SRC		:= $(call SOURCE_PATH,red_black_hmap)
HMAP_HDR		:= $(call HEADER_PATH,red_black_hmap)
HMAP_OBJ		:= $(call OBJECT_PATH,red_black_hmap)
HMAP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hmap)
HMAP_ST_LIB		:= $(call STATIC_LIBRARY_PATH,red_black_hmap)
HMAP_SH_LIB		:= $(call SHARED_LIBRARY_PATH,red_black_hmap)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_OBJ_PREQS		:= $(HMAP_SRC)					\
			   $(HMAP_HDR)					\
		   	   $(HNODE_FACTORY_HDR)				\
		   	   $(HITOR_HDR)					\
		   	   $(HINSERT_HDR)				\
		   	   $(HPUT_HDR)					\
		   	   $(HUPDATE_HDR)				\
		   	   $(HADD_HDR)					\
		   	   $(HDELETE_HDR)				\
		   	   $(HREMOVE_HDR)				\
		   	   $(HDROP_HDR)					\
		   	   $(HPLUCK_HDR)				\
		   	   $(HFIND_HDR)					\
		   	   $(HFETCH_HDR)				\
		   	   $(HREPLACE_HDR)				\
		   	   $(HEXCHANGE_HDR)				\
		   	   $(HGET_HDR)					\
		   	   $(HSET_HDR)					\
		   	   $(HSWAP_HDR)					\
		   	   $(HCOPY_HDR)					\
		   	   $(HCONGRUENT_HDR)				\
		   	   $(HVERIFY_HDR)				\
		   	   $(HCONCAT_HDR)				\
			   $(MALLOC_HDR)
HMAP_OBJ_GROUP		:= $(HMAP_OBJ)					\
			   $(HNODE_FACTORY_OBJ_GROUP)			\
			   $(HITOR_OBJ_GROUP)				\
			   $(HINSERT_OBJ_GROUP)				\
			   $(HPUT_OBJ_GROUP)				\
			   $(HUPDATE_OBJ_GROUP)				\
			   $(HADD_OBJ_GROUP)				\
			   $(HDELETE_OBJ_GROUP)				\
			   $(HREMOVE_OBJ_GROUP)				\
		   	   $(HDROP_OBJ_GROUP)				\
		   	   $(HPLUCK_OBJ_GROUP)				\
			   $(HFIND_OBJ_GROUP)				\
			   $(HFETCH_OBJ_GROUP)				\
			   $(HREPLACE_OBJ_GROUP)			\
			   $(HEXCHANGE_OBJ_GROUP)			\
			   $(HGET_OBJ_GROUP)				\
			   $(HSET_OBJ_GROUP)				\
			   $(HSWAP_OBJ_GROUP)				\
		   	   $(HCOPY_OBJ_GROUP)				\
			   $(HCONGRUENT_OBJ_GROUP)			\
			   $(HVERIFY_OBJ_GROUP)				\
			   $(HCONCAT_OBJ_GROUP)
HMAP_PIC_OBJ_PREQS	:= $(HMAP_OBJ_PREQS)
HMAP_PIC_OBJ_GROUP	:= $(HMAP_PIC_OBJ)				\
			   $(HNODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(HITOR_PIC_OBJ_GROUP)			\
			   $(HINSERT_PIC_OBJ_GROUP)			\
			   $(HPUT_PIC_OBJ_GROUP)			\
			   $(HUPDATE_PIC_OBJ_GROUP)			\
			   $(HADD_PIC_OBJ_GROUP)			\
			   $(HDELETE_PIC_OBJ_GROUP)			\
			   $(HREMOVE_PIC_OBJ_GROUP)			\
		   	   $(HDROP_PIC_OBJ_GROUP)			\
		   	   $(HPLUCK_PIC_OBJ_GROUP)			\
			   $(HFIND_PIC_OBJ_GROUP)			\
			   $(HFETCH_PIC_OBJ_GROUP)			\
			   $(HREPLACE_PIC_OBJ_GROUP)			\
			   $(HEXCHANGE_PIC_OBJ_GROUP)			\
			   $(HGET_PIC_OBJ_GROUP)			\
			   $(HSET_PIC_OBJ_GROUP)			\
			   $(HSWAP_PIC_OBJ_GROUP)			\
		   	   $(HCOPY_PIC_OBJ_GROUP)			\
			   $(HCONGRUENT_PIC_OBJ_GROUP)			\
			   $(HVERIFY_PIC_OBJ_GROUP)			\
			   $(HCONCAT_PIC_OBJ_GROUP)
HMAP_ST_LIB_PREQS	:= $(HMAP_OBJ_GROUP)
HMAP_SH_LIB_PREQS	:= $(HMAP_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(HMAP_OBJ)					\
			   $(HMAP_PIC_OBJ)				\
			   $(HMAP_ST_LIB)				\
			   $(HMAP_SH_LIB)



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
# red_black_test
# ──────────────────────────────────────────────────────────────────────────────
TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black)
TEST_HDR		:= $(call TEST_HEADER_PATH,red_black)
TEST_OBJ		:= $(call OBJECT_PATH,red_black)
# ─────────────── target prequisites ───────────────────────────────────────────
TEST_OBJ_PREQS		:= $(TEST_SRC)					\
			   $(TEST_HDR)
TEST_OBJ_GROUP		:= $(TEST_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TEST_OBJ)



# RED_BLACK_TREE TEST MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# red_black_tree_insertion_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_INSERTION_TEST_SRC	      := $(call TEST_SOURCE_PATH,red_black_tree_insertion)
TREE_INSERTION_TRNR_SRC	      := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_insertion)
TREE_INSERTION_TRNR_OBJ	      := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_insertion)
TREE_INSERTION_TEST_OBJ	      := $(call TEST_OBJECT_PATH,red_black_tree_insertion)
TREE_INSERTION_TEST_BIN	      := $(call TEST_BINARY_PATH,red_black_tree_insertion)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_INSERTION_TRNR_SRC_PREQS := $(TREE_INSERTION_TEST_SRC)
TREE_INSERTION_TEST_OBJ_PREQS := $(TREE_INSERTION_TEST_SRC)		\
			   	 $(TREE_HDR)				\
			   	 $(INT_KEY_HDR)				\
			   	 $(TEST_HDR)				\
			   	 $(UNITY_HDR)
TREE_INSERTION_TRNR_OBJ_PREQS := $(TREE_INSERTION_TRNR_SRC)		\
			   	 $(TREE_INSERTION_TEST_OBJ_PREQS)
TREE_INSERTION_TEST_BIN_PREQS := $(TREE_INSERTION_TRNR_OBJ)		\
			   	 $(TREE_INSERTION_TEST_OBJ)		\
				 $(INT_KEY_OBJ_GROUP)			\
				 $(TREE_SH_LIB)				\
				 $(TEST_OBJ)				\
				 $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(TREE_INSERTION_TEST_BIN)
TARGETS			      += $(TREE_INSERTION_TRNR_SRC)		\
			         $(TREE_INSERTION_TRNR_OBJ)		\
			         $(TREE_INSERTION_TEST_OBJ)		\
			         $(TREE_INSERTION_TEST_BIN)


# red_black_tree_comparison_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_COMPARISON_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_tree_comparison)
TREE_COMPARISON_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_comparison)
TREE_COMPARISON_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_comparison)
TREE_COMPARISON_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_tree_comparison)
TREE_COMPARISON_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_tree_comparison)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_COMPARISON_TRNR_SRC_PREQS := $(TREE_COMPARISON_TEST_SRC)
TREE_COMPARISON_TEST_OBJ_PREQS := $(TREE_COMPARISON_TEST_SRC)		\
			   	  $(TREE_HDR)				\
			   	  $(INT_KEY_HDR)			\
			   	  $(TEST_HDR)				\
			   	  $(UNITY_HDR)
TREE_COMPARISON_TRNR_OBJ_PREQS := $(TREE_COMPARISON_TRNR_SRC)		\
			   	  $(TREE_COMPARISON_TEST_OBJ_PREQS)
TREE_COMPARISON_TEST_BIN_PREQS := $(TREE_COMPARISON_TRNR_OBJ)		\
			   	  $(TREE_COMPARISON_TEST_OBJ)		\
				  $(INT_KEY_OBJ_GROUP)			\
				  $(TREE_SH_LIB)			\
				  $(TEST_OBJ)				\
				  $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(TREE_COMPARISON_TEST_BIN)
TARGETS			      += $(TREE_COMPARISON_TRNR_SRC)		\
			         $(TREE_COMPARISON_TRNR_OBJ)		\
			         $(TREE_COMPARISON_TEST_OBJ)		\
			         $(TREE_COMPARISON_TEST_BIN)


# red_black_tree_access_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_ACCESS_TEST_SRC	     := $(call TEST_SOURCE_PATH,red_black_tree_access)
TREE_ACCESS_TRNR_SRC	     := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_access)
TREE_ACCESS_TRNR_OBJ	     := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_access)
TREE_ACCESS_TEST_OBJ	     := $(call TEST_OBJECT_PATH,red_black_tree_access)
TREE_ACCESS_TEST_BIN	     := $(call TEST_BINARY_PATH,red_black_tree_access)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_ACCESS_TRNR_SRC_PREQS   := $(TREE_ACCESS_TEST_SRC)
TREE_ACCESS_TEST_OBJ_PREQS   := $(TREE_ACCESS_TEST_SRC)			\
			   	$(TREE_HDR)				\
			   	$(INT_KEY_HDR)				\
			   	$(TEST_HDR)				\
			   	$(UNITY_HDR)
TREE_ACCESS_TRNR_OBJ_PREQS   := $(TREE_ACCESS_TRNR_SRC)			\
			   	$(TREE_ACCESS_TEST_OBJ_PREQS)
TREE_ACCESS_TEST_BIN_PREQS   := $(TREE_ACCESS_TRNR_OBJ)			\
			   	$(TREE_ACCESS_TEST_OBJ)			\
				$(INT_KEY_OBJ_GROUP)			\
				$(TREE_SH_LIB)				\
				$(TEST_OBJ)				\
				$(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		     += $(TREE_ACCESS_TEST_BIN)
TARGETS			     += $(TREE_ACCESS_TRNR_SRC)			\
			        $(TREE_ACCESS_TRNR_OBJ)			\
			        $(TREE_ACCESS_TEST_OBJ)			\
			        $(TREE_ACCESS_TEST_BIN)


# red_black_tree_traversal_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_TRAVERSAL_TEST_SRC	      := $(call TEST_SOURCE_PATH,red_black_tree_traversal)
TREE_TRAVERSAL_TRNR_SRC	      := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_traversal)
TREE_TRAVERSAL_TRNR_OBJ	      := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_traversal)
TREE_TRAVERSAL_TEST_OBJ	      := $(call TEST_OBJECT_PATH,red_black_tree_traversal)
TREE_TRAVERSAL_TEST_BIN	      := $(call TEST_BINARY_PATH,red_black_tree_traversal)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_TRAVERSAL_TRNR_SRC_PREQS := $(TREE_TRAVERSAL_TEST_SRC)
TREE_TRAVERSAL_TEST_OBJ_PREQS := $(TREE_TRAVERSAL_TEST_SRC)		\
			         $(TREE_HDR)				\
			         $(INT_KEY_HDR)				\
			         $(TEST_HDR)				\
			         $(UNITY_HDR)
TREE_TRAVERSAL_TRNR_OBJ_PREQS := $(TREE_TRAVERSAL_TRNR_SRC)		\
			   	 $(TREE_TRAVERSAL_TEST_OBJ_PREQS)
TREE_TRAVERSAL_TEST_BIN_PREQS := $(TREE_TRAVERSAL_TRNR_OBJ)		\
			         $(TREE_TRAVERSAL_TEST_OBJ)		\
			         $(INT_KEY_OBJ_GROUP)			\
			         $(TREE_SH_LIB)				\
			         $(TEST_OBJ)				\
				 $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(TREE_TRAVERSAL_TEST_BIN)
TARGETS			      += $(TREE_TRAVERSAL_TRNR_SRC)		\
			         $(TREE_TRAVERSAL_TRNR_OBJ)		\
			         $(TREE_TRAVERSAL_TEST_OBJ)		\
			         $(TREE_TRAVERSAL_TEST_BIN)


# red_black_tree_deletion_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_DELETION_TEST_SRC	     := $(call TEST_SOURCE_PATH,red_black_tree_deletion)
TREE_DELETION_TRNR_SRC	     := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_deletion)
TREE_DELETION_TRNR_OBJ	     := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_deletion)
TREE_DELETION_TEST_OBJ	     := $(call TEST_OBJECT_PATH,red_black_tree_deletion)
TREE_DELETION_TEST_BIN	     := $(call TEST_BINARY_PATH,red_black_tree_deletion)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_DELETION_TRNR_SRC_PREQS := $(TREE_DELETION_TEST_SRC)
TREE_DELETION_TEST_OBJ_PREQS := $(TREE_DELETION_TEST_SRC)		\
			   	$(TREE_HDR)				\
			   	$(INT_KEY_HDR)				\
			   	$(TEST_HDR)				\
			   	$(UNITY_HDR)
TREE_DELETION_TRNR_OBJ_PREQS := $(TREE_DELETION_TRNR_SRC)		\
			   	$(TREE_DELETION_TEST_OBJ_PREQS)
TREE_DELETION_TEST_BIN_PREQS := $(TREE_DELETION_TRNR_OBJ)		\
			   	$(TREE_DELETION_TEST_OBJ)		\
				$(INT_KEY_OBJ_GROUP)			\
				$(TREE_SH_LIB)				\
				$(TEST_OBJ)				\
				$(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		     += $(TREE_DELETION_TEST_BIN)
TARGETS			     += $(TREE_DELETION_TRNR_SRC)		\
			        $(TREE_DELETION_TRNR_OBJ)		\
			        $(TREE_DELETION_TEST_OBJ)		\
			        $(TREE_DELETION_TEST_BIN)


# red_black_tree_set_operations_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_SET_OPERATIONS_TEST_SRC	   := $(call TEST_SOURCE_PATH,red_black_tree_set_operations)
TREE_SET_OPERATIONS_TRNR_SRC	   := $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_set_operations)
TREE_SET_OPERATIONS_TRNR_OBJ	   := $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_set_operations)
TREE_SET_OPERATIONS_TEST_OBJ	   := $(call TEST_OBJECT_PATH,red_black_tree_set_operations)
TREE_SET_OPERATIONS_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_tree_set_operations)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_SET_OPERATIONS_TRNR_SRC_PREQS := $(TREE_SET_OPERATIONS_TEST_SRC)
TREE_SET_OPERATIONS_TEST_OBJ_PREQS := $(TREE_SET_OPERATIONS_TEST_SRC)	\
			   	      $(TREE_HDR)			\
			   	      $(INT_KEY_HDR)			\
			   	      $(TEST_HDR)			\
			   	      $(UNITY_HDR)
TREE_SET_OPERATIONS_TRNR_OBJ_PREQS := $(TREE_SET_OPERATIONS_TRNR_SRC)	\
			   	$(TREE_SET_OPERATIONS_TEST_OBJ_PREQS)
TREE_SET_OPERATIONS_TEST_BIN_PREQS := $(TREE_SET_OPERATIONS_TRNR_OBJ)	\
			   	      $(TREE_SET_OPERATIONS_TEST_OBJ)	\
				      $(INT_KEY_OBJ_GROUP)		\
				      $(TREE_SH_LIB)			\
				      $(TEST_OBJ)			\
				      $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		     	   += $(TREE_SET_OPERATIONS_TEST_BIN)
TARGETS			     	   += $(TREE_SET_OPERATIONS_TRNR_SRC)	\
			              $(TREE_SET_OPERATIONS_TRNR_OBJ)	\
			              $(TREE_SET_OPERATIONS_TEST_OBJ)	\
			              $(TREE_SET_OPERATIONS_TEST_BIN)


# red_black_tree_internals_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_INTERNALS_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_tree_internals)
TREE_INTERNALS_TRNR_SRC		:= $(call TEST_RUNNER_SOURCE_PATH,red_black_tree_internals)
TREE_INTERNALS_TRNR_OBJ		:= $(call TEST_RUNNER_OBJECT_PATH,red_black_tree_internals)
TREE_INTERNALS_TEST_OBJ		:= $(call TEST_OBJECT_PATH,red_black_tree_internals)
TREE_INTERNALS_TEST_BIN		:= $(call TEST_BINARY_PATH,red_black_tree_internals)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_INTERNALS_TRNR_SRC_PREQS	:= $(TREE_INTERNALS_TEST_SRC)
TREE_INTERNALS_TEST_OBJ_PREQS	:= $(TREE_INTERNALS_TEST_SRC)		\
			   	   $(TREE_INTERNALS_HDR)		\
			   	   $(TREEIFY_HDR)			\
			   	   $(VERIFY_HDR)			\
			   	   $(INT_KEY_HDR)			\
			   	   $(TEST_HDR)				\
			   	   $(UNITY_HDR)
TREE_INTERNALS_TRNR_OBJ_PREQS   := $(TREE_INTERNALS_TRNR_SRC)		\
			   	   $(TREE_INTERNALS_TEST_OBJ_PREQS)
TREE_INTERNALS_TEST_BIN_PREQS	:= $(TREE_INTERNALS_TRNR_OBJ)		\
			   	   $(TREE_INTERNALS_TEST_OBJ)		\
			   	   $(TREE_INTERNALS_OBJ_GROUP)		\
			   	   $(INT_KEY_OBJ_GROUP)			\
				   $(TREEIFY_OBJ_GROUP)			\
				   $(VERIFY_OBJ_GROUP)			\
				   $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES			+= $(TREE_INTERNALS_TEST_BIN)
TARGETS				+= $(TREE_INTERNALS_TRNR_SRC)		\
			   	   $(TREE_INTERNALS_TRNR_OBJ)		\
			   	   $(TREE_INTERNALS_TEST_OBJ)		\
			   	   $(TREE_INTERNALS_TEST_BIN)



# RED_BLACK_HMAP TEST MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# red_black_hmap_insertion_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_INSERTION_TEST_SRC	      := $(call TEST_SOURCE_PATH,red_black_hmap_insertion)
HMAP_INSERTION_TRNR_SRC	      := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_insertion)
HMAP_INSERTION_TRNR_OBJ	      := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_insertion)
HMAP_INSERTION_TEST_OBJ	      := $(call TEST_OBJECT_PATH,red_black_hmap_insertion)
HMAP_INSERTION_TEST_BIN	      := $(call TEST_BINARY_PATH,red_black_hmap_insertion)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_INSERTION_TRNR_SRC_PREQS := $(HMAP_INSERTION_TEST_SRC)
HMAP_INSERTION_TEST_OBJ_PREQS := $(HMAP_INSERTION_TEST_SRC)		\
			   	 $(HMAP_HDR)				\
			   	 $(TEST_HDR)				\
			   	 $(UNITY_HDR)
HMAP_INSERTION_TRNR_OBJ_PREQS := $(HMAP_INSERTION_TRNR_SRC)		\
			   	 $(HMAP_INSERTION_TEST_OBJ_PREQS)
HMAP_INSERTION_TEST_BIN_PREQS := $(HMAP_INSERTION_TRNR_OBJ)		\
			   	 $(HMAP_INSERTION_TEST_OBJ)		\
				 $(HMAP_SH_LIB)				\
				 $(TEST_OBJ)				\
				 $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(HMAP_INSERTION_TEST_BIN)
TARGETS			      += $(HMAP_INSERTION_TRNR_SRC)		\
			         $(HMAP_INSERTION_TRNR_OBJ)		\
			         $(HMAP_INSERTION_TEST_OBJ)		\
			         $(HMAP_INSERTION_TEST_BIN)


# red_black_hmap_comparison_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_COMPARISON_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_comparison)
HMAP_COMPARISON_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_comparison)
HMAP_COMPARISON_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_comparison)
HMAP_COMPARISON_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_comparison)
HMAP_COMPARISON_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_comparison)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_COMPARISON_TRNR_SRC_PREQS := $(HMAP_COMPARISON_TEST_SRC)
HMAP_COMPARISON_TEST_OBJ_PREQS := $(HMAP_COMPARISON_TEST_SRC)		\
			   	  $(HMAP_HDR)				\
			   	  $(TEST_HDR)				\
			   	  $(UNITY_HDR)
HMAP_COMPARISON_TRNR_OBJ_PREQS := $(HMAP_COMPARISON_TRNR_SRC)		\
			   	  $(HMAP_COMPARISON_TEST_OBJ_PREQS)
HMAP_COMPARISON_TEST_BIN_PREQS := $(HMAP_COMPARISON_TRNR_OBJ)		\
			   	  $(HMAP_COMPARISON_TEST_OBJ)		\
				  $(HMAP_SH_LIB)			\
				  $(TEST_OBJ)				\
				  $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(HMAP_COMPARISON_TEST_BIN)
TARGETS			      += $(HMAP_COMPARISON_TRNR_SRC)		\
			         $(HMAP_COMPARISON_TRNR_OBJ)		\
			         $(HMAP_COMPARISON_TEST_OBJ)		\
			         $(HMAP_COMPARISON_TEST_BIN)


# red_black_hmap_access_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_ACCESS_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_access)
HMAP_ACCESS_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_access)
HMAP_ACCESS_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_access)
HMAP_ACCESS_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_access)
HMAP_ACCESS_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_access)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_ACCESS_TRNR_SRC_PREQS := $(HMAP_ACCESS_TEST_SRC)
HMAP_ACCESS_TEST_OBJ_PREQS := $(HMAP_ACCESS_TEST_SRC)			\
			      $(HMAP_HDR)				\
			      $(TEST_HDR)				\
			      $(UNITY_HDR)
HMAP_ACCESS_TRNR_OBJ_PREQS := $(HMAP_ACCESS_TRNR_SRC)			\
			      $(HMAP_ACCESS_TEST_OBJ_PREQS)
HMAP_ACCESS_TEST_BIN_PREQS := $(HMAP_ACCESS_TRNR_OBJ)			\
			      $(HMAP_ACCESS_TEST_OBJ)			\
			      $(HMAP_SH_LIB)				\
			      $(TEST_OBJ)				\
			      $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		   += $(HMAP_ACCESS_TEST_BIN)
TARGETS			   += $(HMAP_ACCESS_TRNR_SRC)			\
			      $(HMAP_ACCESS_TRNR_OBJ)			\
			      $(HMAP_ACCESS_TEST_OBJ)			\
			      $(HMAP_ACCESS_TEST_BIN)


# red_black_hmap_traversal_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_TRAVERSAL_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_traversal)
HMAP_TRAVERSAL_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_traversal)
HMAP_TRAVERSAL_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_traversal)
HMAP_TRAVERSAL_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_traversal)
HMAP_TRAVERSAL_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_traversal)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_TRAVERSAL_TRNR_SRC_PREQS := $(HMAP_TRAVERSAL_TEST_SRC)
HMAP_TRAVERSAL_TEST_OBJ_PREQS := $(HMAP_TRAVERSAL_TEST_SRC)		\
			         $(HMAP_HDR)				\
			         $(TEST_HDR)				\
			         $(UNITY_HDR)
HMAP_TRAVERSAL_TRNR_OBJ_PREQS := $(HMAP_TRAVERSAL_TRNR_SRC)		\
			         $(HMAP_TRAVERSAL_TEST_OBJ_PREQS)
HMAP_TRAVERSAL_TEST_BIN_PREQS := $(HMAP_TRAVERSAL_TRNR_OBJ)		\
			         $(HMAP_TRAVERSAL_TEST_OBJ)		\
			      	 $(HMAP_SH_LIB)				\
			      	 $(TEST_OBJ)				\
			      	 $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		      += $(HMAP_TRAVERSAL_TEST_BIN)
TARGETS			      += $(HMAP_TRAVERSAL_TRNR_SRC)		\
				 $(HMAP_TRAVERSAL_TRNR_OBJ)		\
				 $(HMAP_TRAVERSAL_TEST_OBJ)		\
				 $(HMAP_TRAVERSAL_TEST_BIN)


# red_black_hmap_deletion_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_DELETION_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_deletion)
HMAP_DELETION_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_deletion)
HMAP_DELETION_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_deletion)
HMAP_DELETION_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_deletion)
HMAP_DELETION_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_deletion)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_DELETION_TRNR_SRC_PREQS := $(HMAP_DELETION_TEST_SRC)
HMAP_DELETION_TEST_OBJ_PREQS := $(HMAP_DELETION_TEST_SRC)		\
			        $(HMAP_HDR)				\
			        $(TEST_HDR)				\
			        $(UNITY_HDR)
HMAP_DELETION_TRNR_OBJ_PREQS := $(HMAP_DELETION_TRNR_SRC)		\
			        $(HMAP_DELETION_TEST_OBJ_PREQS)
HMAP_DELETION_TEST_BIN_PREQS := $(HMAP_DELETION_TRNR_OBJ)		\
			      	$(HMAP_DELETION_TEST_OBJ)		\
			      	$(HMAP_SH_LIB)				\
			      	$(TEST_OBJ)				\
			      	$(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		     += $(HMAP_DELETION_TEST_BIN)
TARGETS			     += $(HMAP_DELETION_TRNR_SRC)		\
			        $(HMAP_DELETION_TRNR_OBJ)		\
			        $(HMAP_DELETION_TEST_OBJ)		\
			        $(HMAP_DELETION_TEST_BIN)


# red_black_hmap_set_operations_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_SET_OPERATIONS_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_set_operations)
HMAP_SET_OPERATIONS_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_set_operations)
HMAP_SET_OPERATIONS_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_set_operations)
HMAP_SET_OPERATIONS_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_set_operations)
HMAP_SET_OPERATIONS_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_set_operations)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_SET_OPERATIONS_TRNR_SRC_PREQS := $(HMAP_SET_OPERATIONS_TEST_SRC)
HMAP_SET_OPERATIONS_TEST_OBJ_PREQS := $(HMAP_SET_OPERATIONS_TEST_SRC)	\
			              $(HMAP_HDR)			\
			              $(TEST_HDR)			\
			              $(UNITY_HDR)
HMAP_SET_OPERATIONS_TRNR_OBJ_PREQS := $(HMAP_SET_OPERATIONS_TRNR_SRC)	\
			              $(HMAP_SET_OPERATIONS_TEST_OBJ_PREQS)
HMAP_SET_OPERATIONS_TEST_BIN_PREQS := $(HMAP_SET_OPERATIONS_TRNR_OBJ)	\
			      	      $(HMAP_SET_OPERATIONS_TEST_OBJ)	\
			      	      $(HMAP_SH_LIB)			\
			      	      $(TEST_OBJ)			\
			      	      $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		     	   += $(HMAP_SET_OPERATIONS_TEST_BIN)
TARGETS			     	   += $(HMAP_SET_OPERATIONS_TRNR_SRC)	\
			              $(HMAP_SET_OPERATIONS_TRNR_OBJ)	\
			              $(HMAP_SET_OPERATIONS_TEST_OBJ)	\
			              $(HMAP_SET_OPERATIONS_TEST_BIN)


# red_black_hmap_internals_test
# ──────────────────────────────────────────────────────────────────────────────
HMAP_INTERNALS_TEST_SRC       := $(call TEST_SOURCE_PATH,red_black_hmap_internals)
HMAP_INTERNALS_TRNR_SRC       := $(call TEST_RUNNER_SOURCE_PATH,red_black_hmap_internals)
HMAP_INTERNALS_TRNR_OBJ       := $(call TEST_RUNNER_OBJECT_PATH,red_black_hmap_internals)
HMAP_INTERNALS_TEST_OBJ       := $(call TEST_OBJECT_PATH,red_black_hmap_internals)
HMAP_INTERNALS_TEST_BIN       := $(call TEST_BINARY_PATH,red_black_hmap_internals)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_INTERNALS_TRNR_SRC_PREQS := $(HMAP_INTERNALS_TEST_SRC)
HMAP_INTERNALS_TEST_OBJ_PREQS := $(HMAP_INTERNALS_TEST_SRC)		\
				 $(HKEY_HDR)				\
				 $(TEST_HDR)				\
				 $(UNITY_HDR)
HMAP_INTERNALS_TRNR_OBJ_PREQS := $(HMAP_INTERNALS_TRNR_SRC)		\
			      	 $(HMAP_INTERNALS_TEST_OBJ_PREQS)
HMAP_INTERNALS_TEST_BIN_PREQS := $(HMAP_INTERNALS_TRNR_OBJ)		\
			      	 $(HMAP_INTERNALS_TEST_OBJ)		\
			      	 $(HKEY_OBJ_GROUP)			\
				 $(TEST_OBJ)				\
				 $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		    += $(HMAP_INTERNALS_TEST_BIN)
TARGETS			    += $(HMAP_INTERNALS_TRNR_SRC)			\
			       $(HMAP_INTERNALS_TRNR_OBJ)			\
			       $(HMAP_INTERNALS_TEST_OBJ)			\
			       $(HMAP_INTERNALS_TEST_BIN)


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
.PHONY: all run_tests clean

all: $(TARGETS)

run_tests: $(TEST_BINARIES)
	$(RUN_TESTS)

clean:
	$(RM) $(RM_FLAGS) $(TARGETS)


# RED_BLACK_TREE
# ──────────────────────────────────────────────────────────────────────────────
$(TREE_ST_LIB): $(TREE_ST_LIB_PREQS)
	$(AR) $(AR_FLAGS) $@ $^
$(TREE_SH_LIB): $(TREE_SH_LIB_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_SH_FLAGS) -o $@
$(TREE_OBJ): $(TREE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(TREE_PIC_OBJ): $(TREE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


# RED_BLACK_TREE MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(NODE_FACTORY_OBJ): $(NODE_FACTORY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(NODE_FACTORY_PIC_OBJ): $(NODE_FACTORY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ITOR_OBJ): $(ITOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(ITOR_PIC_OBJ): $(ITOR_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(INSERT_OBJ): $(INSERT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(INSERT_PIC_OBJ): $(INSERT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(PUT_OBJ): $(PUT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(PUT_PIC_OBJ): $(PUT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(UPDATE_OBJ): $(UPDATE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(UPDATE_PIC_OBJ): $(UPDATE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ADD_OBJ): $(ADD_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(ADD_PIC_OBJ): $(ADD_PIC_OBJ_PREQS)
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

$(DROP_OBJ): $(DROP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(DROP_PIC_OBJ): $(DROP_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(PLUCK_OBJ): $(PLUCK_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(PLUCK_PIC_OBJ): $(PLUCK_PIC_OBJ_PREQS)
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

$(REPLACE_OBJ): $(REPLACE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(REPLACE_PIC_OBJ): $(REPLACE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(EXCHANGE_OBJ): $(EXCHANGE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(EXCHANGE_PIC_OBJ): $(EXCHANGE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(GET_OBJ): $(GET_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(GET_PIC_OBJ): $(GET_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(SET_OBJ): $(SET_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(SET_PIC_OBJ): $(SET_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(SWAP_OBJ): $(SWAP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(SWAP_PIC_OBJ): $(SWAP_PIC_OBJ_PREQS)
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

$(TREEIFY_OBJ): $(TREEIFY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(TREEIFY_PIC_OBJ): $(TREEIFY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(COPY_OBJ): $(COPY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(COPY_PIC_OBJ): $(COPY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(CONGRUENT_OBJ): $(CONGRUENT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(CONGRUENT_PIC_OBJ): $(CONGRUENT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


# RED_BLACK_HMAP
# ──────────────────────────────────────────────────────────────────────────────
$(HMAP_ST_LIB): $(HMAP_ST_LIB_PREQS)
	$(AR) $(AR_FLAGS) $@ $^
$(HMAP_SH_LIB): $(HMAP_SH_LIB_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_SH_FLAGS) -o $@
$(HMAP_OBJ): $(HMAP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HMAP_PIC_OBJ): $(HMAP_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


# RED_BLACK_HMAP MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(HNODE_FACTORY_OBJ): $(HNODE_FACTORY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HNODE_FACTORY_PIC_OBJ): $(HNODE_FACTORY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HITOR_OBJ): $(HITOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HITOR_PIC_OBJ): $(HITOR_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HINSERT_OBJ): $(HINSERT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HINSERT_PIC_OBJ): $(HINSERT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HPUT_OBJ): $(HPUT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HPUT_PIC_OBJ): $(HPUT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HUPDATE_OBJ): $(HUPDATE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HUPDATE_PIC_OBJ): $(HUPDATE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HADD_OBJ): $(HADD_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HADD_PIC_OBJ): $(HADD_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HCORRECT_OBJ): $(HCORRECT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HCORRECT_PIC_OBJ): $(HCORRECT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HDELETE_OBJ): $(HDELETE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HDELETE_PIC_OBJ): $(HDELETE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HREMOVE_OBJ): $(HREMOVE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HREMOVE_PIC_OBJ): $(HREMOVE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HDROP_OBJ): $(HDROP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HDROP_PIC_OBJ): $(HDROP_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HPLUCK_OBJ): $(HPLUCK_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HPLUCK_PIC_OBJ): $(HPLUCK_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HRESTORE_OBJ): $(HRESTORE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HRESTORE_PIC_OBJ): $(HRESTORE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HFIND_OBJ): $(HFIND_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HFIND_PIC_OBJ): $(HFIND_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HFETCH_OBJ): $(HFETCH_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HFETCH_PIC_OBJ): $(HFETCH_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HREPLACE_OBJ): $(HREPLACE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HREPLACE_PIC_OBJ): $(HREPLACE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HEXCHANGE_OBJ): $(HEXCHANGE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HEXCHANGE_PIC_OBJ): $(HEXCHANGE_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HGET_OBJ): $(HGET_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HGET_PIC_OBJ): $(HGET_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HSET_OBJ): $(HSET_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HSET_PIC_OBJ): $(HSET_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HSWAP_OBJ): $(HSWAP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HSWAP_PIC_OBJ): $(HSWAP_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HCOUNT_OBJ): $(HCOUNT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HCOUNT_PIC_OBJ): $(HCOUNT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HVERIFY_OBJ): $(HVERIFY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HVERIFY_PIC_OBJ): $(HVERIFY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HCOPY_OBJ): $(HCOPY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HCOPY_PIC_OBJ): $(HCOPY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HCONGRUENT_OBJ): $(HCONGRUENT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HCONGRUENT_PIC_OBJ): $(HCONGRUENT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HCONCAT_OBJ): $(HCONCAT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HCONCAT_PIC_OBJ): $(HCONCAT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HKEY_OBJ): $(HKEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HKEY_PIC_OBJ): $(HKEY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


# KEY_ACCESSOR MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(INT_KEY_OBJ): $(INT_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(STR_KEY_OBJ): $(STR_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


# RED_BLACK_TREE_TEST MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(TREE_INSERTION_TEST_BIN): $(TREE_INSERTION_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_INSERTION_TRNR_OBJ): $(TREE_INSERTION_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INSERTION_TEST_OBJ): $(TREE_INSERTION_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INSERTION_TRNR_SRC): $(TREE_INSERTION_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_COMPARISON_TEST_BIN): $(TREE_COMPARISON_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_COMPARISON_TRNR_OBJ): $(TREE_COMPARISON_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_COMPARISON_TEST_OBJ): $(TREE_COMPARISON_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_COMPARISON_TRNR_SRC): $(TREE_COMPARISON_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_ACCESS_TEST_BIN): $(TREE_ACCESS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_ACCESS_TRNR_OBJ): $(TREE_ACCESS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_ACCESS_TEST_OBJ): $(TREE_ACCESS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_ACCESS_TRNR_SRC): $(TREE_ACCESS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_TRAVERSAL_TEST_BIN): $(TREE_TRAVERSAL_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_TRAVERSAL_TRNR_OBJ): $(TREE_TRAVERSAL_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_TRAVERSAL_TEST_OBJ): $(TREE_TRAVERSAL_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_TRAVERSAL_TRNR_SRC): $(TREE_TRAVERSAL_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_DELETION_TEST_BIN): $(TREE_DELETION_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_DELETION_TRNR_OBJ): $(TREE_DELETION_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_DELETION_TEST_OBJ): $(TREE_DELETION_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_DELETION_TRNR_SRC): $(TREE_DELETION_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_SET_OPERATIONS_TEST_BIN): $(TREE_SET_OPERATIONS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_SET_OPERATIONS_TRNR_OBJ): $(TREE_SET_OPERATIONS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_SET_OPERATIONS_TEST_OBJ): $(TREE_SET_OPERATIONS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_SET_OPERATIONS_TRNR_SRC): $(TREE_SET_OPERATIONS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(TREE_INTERNALS_TEST_BIN): $(TREE_INTERNALS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_INTERNALS_TRNR_OBJ): $(TREE_INTERNALS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INTERNALS_TEST_OBJ): $(TREE_INTERNALS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INTERNALS_TRNR_SRC): $(TREE_INTERNALS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@


# RED_BLACK_HMAP_TEST MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(HMAP_INSERTION_TEST_BIN): $(HMAP_INSERTION_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_INSERTION_TRNR_OBJ): $(HMAP_INSERTION_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_INSERTION_TEST_OBJ): $(HMAP_INSERTION_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_INSERTION_TRNR_SRC): $(HMAP_INSERTION_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_COMPARISON_TEST_BIN): $(HMAP_COMPARISON_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_COMPARISON_TRNR_OBJ): $(HMAP_COMPARISON_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_COMPARISON_TEST_OBJ): $(HMAP_COMPARISON_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_COMPARISON_TRNR_SRC): $(HMAP_COMPARISON_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_ACCESS_TEST_BIN): $(HMAP_ACCESS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_ACCESS_TRNR_OBJ): $(HMAP_ACCESS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_ACCESS_TEST_OBJ): $(HMAP_ACCESS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_ACCESS_TRNR_SRC): $(HMAP_ACCESS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_TRAVERSAL_TEST_BIN): $(HMAP_TRAVERSAL_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_TRAVERSAL_TRNR_OBJ): $(HMAP_TRAVERSAL_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_TRAVERSAL_TEST_OBJ): $(HMAP_TRAVERSAL_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_TRAVERSAL_TRNR_SRC): $(HMAP_TRAVERSAL_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_DELETION_TEST_BIN): $(HMAP_DELETION_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_DELETION_TRNR_OBJ): $(HMAP_DELETION_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_DELETION_TEST_OBJ): $(HMAP_DELETION_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_DELETION_TRNR_SRC): $(HMAP_DELETION_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_SET_OPERATIONS_TEST_BIN): $(HMAP_SET_OPERATIONS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_SET_OPERATIONS_TRNR_OBJ): $(HMAP_SET_OPERATIONS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_SET_OPERATIONS_TEST_OBJ): $(HMAP_SET_OPERATIONS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_SET_OPERATIONS_TRNR_SRC): $(HMAP_SET_OPERATIONS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(HMAP_INTERNALS_TEST_BIN): $(HMAP_INTERNALS_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(HMAP_INTERNALS_TRNR_OBJ): $(HMAP_INTERNALS_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_INTERNALS_TEST_OBJ): $(HMAP_INTERNALS_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(HMAP_INTERNALS_TRNR_SRC): $(HMAP_INTERNALS_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@



# RED_BLACK_TEST OBJECT
# ──────────────────────────────────────────────────────────────────────────────
$(TEST_OBJ): $(TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@



# UNITY OBJECT
# ──────────────────────────────────────────────────────────────────────────────
$(UNITY_OBJ): $(UNITY_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) -c $< -o $@



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
