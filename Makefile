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
HEADER_DIR             := include# library header files
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
HEADER_PATH             = $(call HEADER_FILE_PATH,$(HEADER_DIR),$1)
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
TEST_FILES_GLOB		:= $(call BINARY_FILE_PATH,$(BINARY_DIR),*_test)
ifeq (T,$(SYSTEM_WINDOWS))
RUN_TESTS		:= FORFILES /M $(TEST_FILES_GLOB) /C "cmd /c @file"

else
RUN_TESTS		:= for test in $(TEST_FILES_GLOB); do "./$$test"; done
endif
# ─────────────── target prequisites ───────────────────────────────────────────
UNITY_OBJ_PREQS		:= $(UNITY_SRC)					\
			   $(UNITY_HDR)					\
			   $(UNITY_INTERNALS_HDR)			\
			   $(UNITY_CONFIG_HDR)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(UNITY_OBJ)




# RED_BLACK MODULESS
# ══════════════════════════════════════════════════════════════════════════════
# Independent Headers
# ──────────────────────────────────────────────────────────────────────────────
STACK_COUNT_HDR := $(call HEADER_PATH,red_black_stack_count)
COMPARATOR_HDR  := $(call HEADER_PATH,red_black_comparator)
JUMP_HDR        := $(call HEADER_PATH,red_black_jump)
MALLOC_HDR      := $(call HEADER_PATH,red_black_malloc)
NODE_HDR        := $(call HEADER_PATH,red_black_node)
HNODE_HDR       := $(call HEADER_PATH,red_black_hnode)
PRINT_TYPES_HDR := $(call HEADER_PATH,red_black_print_types)
RED_BLACK_HDR   := $(call HEADER_PATH,red_black)


# red_black_node_factory
# ──────────────────────────────────────────────────────────────────────────────
NODE_FACTORY_SRC		:= $(call SOURCE_PATH,red_black_node_factory)
NODE_FACTORY_HDR		:= $(call HEADER_PATH,red_black_node_factory)
NODE_FACTORY_OBJ		:= $(call OBJECT_PATH,red_black_node_factory)
NODE_FACTORY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_node_factory)
# ─────────────── target prequisites ───────────────────────────────────────────
NODE_FACTORY_OBJ_PREQS		:= $(NODE_FACTORY_SRC)			\
			   	   $(NODE_FACTORY_HDR)			\
			   	   $(JUMP_HDR)				\
			   	   $(NODE_HDR)				\
			   	   $(HNODE_HDR)				\
			   	   $(MALLOC_HDR)
NODE_FACTORY_OBJ_GROUP		:= $(NODE_FACTORY_OBJ)
NODE_FACTORY_PIC_OBJ_PREQS	:= $(NODE_FACTORY_OBJ_PREQS)
NODE_FACTORY_PIC_OBJ_GROUP 	:= $(NODE_FACTORY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(NODE_FACTORY_OBJ)			\
			   	   $(NODE_FACTORY_PIC_OBJ)


# red_black_link_offset
# ──────────────────────────────────────────────────────────────────────────────
LINK_OFFSET_SRC			:= $(call SOURCE_PATH,red_black_link_offset)
LINK_OFFSET_HDR			:= $(call HEADER_PATH,red_black_link_offset)
LINK_OFFSET_OBJ			:= $(call OBJECT_PATH,red_black_link_offset)
LINK_OFFSET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_link_offset)
# ─────────────── target prequisites ───────────────────────────────────────────
LINK_OFFSET_OBJ_PREQS		:= $(LINK_OFFSET_SRC)			\
			   	   $(LINK_OFFSET_HDR)			\
			   	   $(NODE_HDR)
LINK_OFFSET_OBJ_GROUP		:= $(LINK_OFFSET_OBJ)
LINK_OFFSET_PIC_OBJ_PREQS	:= $(LINK_OFFSET_OBJ_PREQS)
LINK_OFFSET_PIC_OBJ_GROUP	:= $(LINK_OFFSET_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(LINK_OFFSET_OBJ)			\
				   $(LINK_OFFSET_PIC_OBJ)


# red_black_etor
# ──────────────────────────────────────────────────────────────────────────────
ETOR_SRC		:= $(call SOURCE_PATH,red_black_etor)
ETOR_HDR		:= $(call HEADER_PATH,red_black_etor)
ETOR_OBJ		:= $(call OBJECT_PATH,red_black_etor)
ETOR_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_etor)
# ─────────────── target prequisites ───────────────────────────────────────────
ETOR_OBJ_PREQS		:= $(ETOR_SRC)					\
			   $(ETOR_HDR)					\
			   $(LINK_OFFSET_HDR)				\
			   $(STACK_COUNT_HDR)
ETOR_OBJ_GROUP		:= $(ETOR_OBJ)					\
			   $(LINK_OFFSET_OBJ_GROUP)
ETOR_PIC_OBJ_PREQS	:= $(ETOR_OBJ_PREQS)
ETOR_PIC_OBJ_GROUP	:= $(ETOR_PIC_OBJ)				\
			   $(LINK_OFFSET_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(ETOR_OBJ)					\
			   $(ETOR_PIC_OBJ)

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
PUT_SRC			:= $(call SOURCE_PATH,red_black_put)
PUT_HDR			:= $(call HEADER_PATH,red_black_put)
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
UPDATE_SRC		:= $(call SOURCE_PATH,red_black_update)
UPDATE_HDR		:= $(call HEADER_PATH,red_black_update)
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
ADD_SRC			:= $(call SOURCE_PATH,red_black_add)
ADD_HDR			:= $(call HEADER_PATH,red_black_add)
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
RESTORE_SRC		:= $(call SOURCE_PATH,red_black_restore)
RESTORE_HDR		:= $(call HEADER_PATH,red_black_restore)
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
DELETE_SRC		:= $(call SOURCE_PATH,red_black_delete)
DELETE_HDR		:= $(call HEADER_PATH,red_black_delete)
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
REMOVE_SRC		:= $(call SOURCE_PATH,red_black_remove)
REMOVE_HDR		:= $(call HEADER_PATH,red_black_remove)
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
DROP_SRC		:= $(call SOURCE_PATH,red_black_drop)
DROP_HDR		:= $(call HEADER_PATH,red_black_drop)
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
PLUCK_SRC		:= $(call SOURCE_PATH,red_black_pluck)
PLUCK_HDR		:= $(call HEADER_PATH,red_black_pluck)
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


# red_black_replace
# ──────────────────────────────────────────────────────────────────────────────
REPLACE_SRC		:= $(call SOURCE_PATH,red_black_replace)
REPLACE_HDR		:= $(call HEADER_PATH,red_black_replace)
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
EXCHANGE_SRC		:= $(call SOURCE_PATH,red_black_exchange)
EXCHANGE_HDR		:= $(call HEADER_PATH,red_black_exchange)
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
GET_SRC			:= $(call SOURCE_PATH,red_black_get)
GET_HDR			:= $(call HEADER_PATH,red_black_get)
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
SET_SRC			:= $(call SOURCE_PATH,red_black_set)
SET_HDR			:= $(call HEADER_PATH,red_black_set)
SET_OBJ			:= $(call OBJECT_PATH,red_black_set)
SET_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_set)
# ─────────────── tarset prequisites ───────────────────────────────────────────
SET_OBJ_PREQS		:= $(SET_SRC)					\
			   $(SET_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
SET_OBJ_GROUP		:= $(SET_OBJ)
SET_PIC_OBJ_PREQS	:= $(SET_OBJ_PREQS)
SET_PIC_OBJ_GROUP	:= $(SET_PIC_OBJ)
# ─────────────── tarsets ──────────────────────────────────────────────────────
TARGETS			+= $(SET_OBJ)					\
			   $(SET_PIC_OBJ)


# red_black_swap
# ──────────────────────────────────────────────────────────────────────────────
SWAP_SRC		:= $(call SOURCE_PATH,red_black_swap)
SWAP_HDR		:= $(call HEADER_PATH,red_black_swap)
SWAP_OBJ		:= $(call OBJECT_PATH,red_black_swap)
SWAP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_swap)
# ─────────────── tarswap prequisites ───────────────────────────────────────────
SWAP_OBJ_PREQS		:= $(SWAP_SRC)					\
			   $(SWAP_HDR)					\
			   $(NODE_HDR)					\
			   $(COMPARATOR_HDR)
SWAP_OBJ_GROUP		:= $(SWAP_OBJ)
SWAP_PIC_OBJ_PREQS	:= $(SWAP_OBJ_PREQS)
SWAP_PIC_OBJ_GROUP	:= $(SWAP_PIC_OBJ)
# ─────────────── tarswaps ──────────────────────────────────────────────────────
TARGETS			+= $(SWAP_OBJ)					\
			   $(SWAP_PIC_OBJ)


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


# red_black_copy
# ──────────────────────────────────────────────────────────────────────────────
COPY_SRC		:= $(call SOURCE_PATH,red_black_copy)
COPY_HDR		:= $(call HEADER_PATH,red_black_copy)
COPY_OBJ		:= $(call OBJECT_PATH,red_black_copy)
COPY_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_copy)
# ─────────────── target prequisites ───────────────────────────────────────────
COPY_OBJ_PREQS		:= $(COPY_SRC)					\
			   $(COPY_HDR)					\
			   $(NODE_HDR)					\
			   $(HNODE_HDR)
COPY_OBJ_GROUP		:= $(COPY_OBJ)
COPY_PIC_OBJ_PREQS	:= $(COPY_OBJ_PREQS)
COPY_PIC_OBJ_GROUP	:= $(COPY_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(COPY_OBJ)					\
			   $(COPY_PIC_OBJ)


# red_black_congruent
# ──────────────────────────────────────────────────────────────────────────────
CONGRUENT_SRC		:= $(call SOURCE_PATH,red_black_congruent)
CONGRUENT_HDR		:= $(call HEADER_PATH,red_black_congruent)
CONGRUENT_OBJ		:= $(call OBJECT_PATH,red_black_congruent)
CONGRUENT_PIC_OBJ	:= $(call PIC_OBJECT_PATH,red_black_congruent)
# ─────────────── target prequisites ───────────────────────────────────────────
CONGRUENT_OBJ_PREQS	:= $(CONGRUENT_SRC)				\
			   $(CONGRUENT_HDR)				\
			   $(NODE_HDR)					\
			   $(HNODE_HDR)
CONGRUENT_OBJ_GROUP	:= $(CONGRUENT_OBJ)
CONGRUENT_PIC_OBJ_PREQS	:= $(CONGRUENT_OBJ_PREQS)
CONGRUENT_PIC_OBJ_GROUP	:= $(CONGRUENT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(CONGRUENT_OBJ)				\
			   $(CONGRUENT_PIC_OBJ)


ifneq (T,$(SYSTEM_WINDOWS))
# red_black_lock
# ──────────────────────────────────────────────────────────────────────────────
LOCK_SRC		:= $(call SOURCE_PATH,red_black_lock)
LOCK_HDR		:= $(call HEADER_PATH,red_black_lock)
LOCK_OBJ		:= $(call OBJECT_PATH,red_black_lock)
LOCK_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_lock)
# ─────────────── target prequisites ───────────────────────────────────────────
LOCK_OBJ_PREQS		:= $(LOCK_SRC)					\
			   $(LOCK_HDR)
LOCK_OBJ_GROUP		:= $(LOCK_OBJ)
LOCK_PIC_OBJ_PREQS	:= $(LOCK_OBJ_PREQS)
LOCK_PIC_OBJ_GROUP	:= $(LOCK_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(LOCK_OBJ)					\
			   $(LOCK_PIC_OBJ)
endif


# red_black_hkey
# ──────────────────────────────────────────────────────────────────────────────
HKEY_SRC		:= $(call SOURCE_PATH,red_black_hkey)
HKEY_HDR		:= $(call HEADER_PATH,red_black_hkey)
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


# red_black_hmap_count
# ──────────────────────────────────────────────────────────────────────────────
HMAP_COUNT_SRC			:= $(call SOURCE_PATH,red_black_hmap_count)
HMAP_COUNT_HDR			:= $(call HEADER_PATH,red_black_hmap_count)
HMAP_COUNT_OBJ			:= $(call OBJECT_PATH,red_black_hmap_count)
HMAP_COUNT_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_hmap_count)
# ─────────────── target prequisites ───────────────────────────────────────────
HMAP_COUNT_OBJ_PREQS		:= $(HMAP_COUNT_SRC)			\
			   	   $(HMAP_COUNT_HDR)
HMAP_COUNT_OBJ_GROUP		:= $(HMAP_COUNT_OBJ)
HMAP_COUNT_PIC_OBJ_PREQS	:= $(HMAP_COUNT_OBJ_PREQS)
HMAP_COUNT_PIC_OBJ_GROUP	:= $(HMAP_COUNT_PIC_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(HMAP_COUNT_OBJ)			\
			   	   $(HMAP_COUNT_PIC_OBJ)


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
		   	   $(ETOR_HDR)				\
		   	   $(PRINT_TYPES_HDR)				\
		   	   $(INSERT_HDR)				\
		   	   $(PUT_HDR)					\
		   	   $(UPDATE_HDR)				\
		   	   $(ADD_HDR)					\
		   	   $(DELETE_HDR)				\
		   	   $(REMOVE_HDR)				\
		   	   $(DROP_HDR)				\
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
		   	   $(VERIFY_HDR)				\
		   	   $(PRINT_HDR)
TREE_OBJ_GROUP		:= $(TREE_OBJ)					\
			   $(NODE_FACTORY_OBJ_GROUP)			\
			   $(ETOR_OBJ_GROUP)				\
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
			   $(PRINT_OBJ_GROUP)
TREE_PIC_OBJ_PREQS	:= $(TREE_OBJ_PREQS)
TREE_PIC_OBJ_GROUP	:= $(TREE_PIC_OBJ)				\
			   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
			   $(ETOR_PIC_OBJ_GROUP)			\
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
			   $(PRINT_PIC_OBJ_GROUP)
TREE_ST_LIB_PREQS	:= $(TREE_OBJ_GROUP)
TREE_SH_LIB_PREQS	:= $(TREE_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREE_OBJ)					\
			   $(TREE_PIC_OBJ)				\
			   $(TREE_ST_LIB)				\
			   $(TREE_SH_LIB)



# red_black_lhmap
# ──────────────────────────────────────────────────────────────────────────────
LHMAP_SRC		:= $(call SOURCE_PATH,red_black_lhmap)
LHMAP_HDR		:= $(call HEADER_PATH,red_black_lhmap)
LHMAP_OBJ		:= $(call OBJECT_PATH,red_black_lhmap)
LHMAP_PIC_OBJ		:= $(call PIC_OBJECT_PATH,red_black_lhmap)
LHMAP_ST_LIB		:= $(call STATIC_LIBRARY_PATH,red_black_lhmap)
LHMAP_SH_LIB		:= $(call SHARED_LIBRARY_PATH,red_black_lhmap)
# ─────────────── target prequisites ───────────────────────────────────────────
LHMAP_OBJ_PREQS		:= $(LHMAP_SRC)					\
			   $(LHMAP_HDR)					\
			   $(LOCK_HDR)					\
		   	   $(NODE_FACTORY_HDR)				\
			   $(HMAP_COUNT_HDR)				\
		   	   $(ETOR_HDR)				\
		   	   $(HASH_NODE_HDR)				\
		   	   $(INSERT_HDR)				\
		   	   $(UPDATE_HDR)				\
		   	   $(DELETE_HDR)				\
		   	   $(REMOVE_HDR)				\
		   	   $(DROP_HDR)					\
		   	   $(FIND_HDR)					\
		   	   $(FETCH_HDR)					\
		   	   $(VERIFY_HDR)				\
		   	   $(CONCAT_HDR)				\
		   	   $(ADD_HDR)					\
			   $(MALLOC_HDR)
LHMAP_OBJ_GROUP		:= $(LHMAP_OBJ)					\
			   $(LOCK_OBJ_GROUP)				\
		   	   $(NODE_FACTORY_OBJ_GROUP)			\
		   	   $(HMAP_COUNT_OBJ_GROUP)			\
		   	   $(ETOR_OBJ_GROUP)				\
		   	   $(HKEY_OBJ_GROUP)				\
		   	   $(INSERT_OBJ_GROUP)				\
		   	   $(UPDATE_OBJ_GROUP)				\
		   	   $(DELETE_OBJ_GROUP)				\
		   	   $(REMOVE_OBJ_GROUP)				\
		   	   $(DROP_OBJ_GROUP)				\
		   	   $(FIND_OBJ_GROUP)				\
		   	   $(FETCH_OBJ_GROUP)				\
		   	   $(VERIFY_OBJ_GROUP)				\
		   	   $(CONCAT_OBJ_GROUP)				\
		   	   $(ADD_OBJ_GROUP)
LHMAP_PIC_OBJ_PREQS	:= $(LHMAP_OBJ_PREQS)
LHMAP_PIC_OBJ_GROUP	:= $(LHMAP_PIC_OBJ)				\
			   $(LOCK_PIC_OBJ_GROUP)			\
		   	   $(NODE_FACTORY_PIC_OBJ_GROUP)		\
		   	   $(HMAP_COUNT_PIC_OBJ_GROUP)			\
		   	   $(ETOR_PIC_OBJ_GROUP)			\
		   	   $(HKEY_PIC_OBJ_GROUP)			\
		   	   $(INSERT_PIC_OBJ_GROUP)			\
		   	   $(UPDATE_PIC_OBJ_GROUP)			\
		   	   $(DELETE_PIC_OBJ_GROUP)			\
		   	   $(REMOVE_PIC_OBJ_GROUP)			\
		   	   $(DROP_PIC_OBJ_GROUP)			\
		   	   $(FIND_PIC_OBJ_GROUP)			\
		   	   $(FETCH_PIC_OBJ_GROUP)			\
		   	   $(VERIFY_PIC_OBJ_GROUP)			\
		   	   $(CONCAT_PIC_OBJ_GROUP)			\
		   	   $(ADD_PIC_OBJ_GROUP)
LHMAP_ST_LIB_PREQS	:= $(LHMAP_OBJ_GROUP)
LHMAP_SH_LIB_PREQS	:= $(LHMAP_PIC_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(LHMAP_OBJ)					\
			   $(LHMAP_PIC_OBJ)				\
			   $(LHMAP_ST_LIB)				\
			   $(LHMAP_SH_LIB)



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


# red_black_mt_test
# ──────────────────────────────────────────────────────────────────────────────
MT_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_mt)
MT_TEST_HDR		:= $(call TEST_HEADER_PATH,red_black_mt)
MT_TEST_OBJ		:= $(call TEST_OBJECT_PATH,red_black_mt)
# ─────────────── target prequisites ───────────────────────────────────────────
MT_TEST_OBJ_PREQS	:= $(MT_TEST_SRC)				\
			   $(MT_TEST_HDR)				\
			   $(TEST_HDR)
MT_TEST_OBJ_GROUP	:= $(MT_TEST_OBJ)				\
			   $(TEST_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(MT_TEST_OBJ)


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


# red_black_concat_test
# ──────────────────────────────────────────────────────────────────────────────
CONCAT_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_concat)
CONCAT_TRNR_SRC		:= $(call TEST_RUNNER_SOURCE_PATH,red_black_concat)
CONCAT_TRNR_OBJ		:= $(call TEST_RUNNER_OBJECT_PATH,red_black_concat)
CONCAT_TEST_OBJ		:= $(call TEST_OBJECT_PATH,red_black_concat)
CONCAT_TEST_BIN		:= $(call TEST_BINARY_PATH,red_black_concat)
# ─────────────── target prequisites ───────────────────────────────────────────
CONCAT_TRNR_SRC_PREQS	:= $(CONCAT_TEST_SRC)
CONCAT_TEST_OBJ_PREQS	:= $(CONCAT_TEST_SRC)				\
			   $(CONCAT_HDR)				\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)				\
			   $(TEST_HDR)					\
			   $(UNITY_HDR)
CONCAT_TRNR_OBJ_PREQS   := $(CONCAT_TRNR_SRC)				\
			   $(CONCAT_TEST_OBJ_PREQS)
CONCAT_TEST_BIN_PREQS	:= $(CONCAT_TRNR_OBJ)				\
			   $(CONCAT_TEST_OBJ)				\
			   $(CONCAT_OBJ_GROUP)				\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_SH_LIB)				\
			   $(TEST_OBJ)					\
			   $(UNITY_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TEST_BINARIES		+= $(CONCAT_TEST_BIN)
TARGETS			+= $(CONCAT_TRNR_SRC)				\
			   $(CONCAT_TRNR_OBJ)				\
			   $(CONCAT_TEST_OBJ)				\
			   $(CONCAT_TEST_BIN)


# red_black_tree_test
# ──────────────────────────────────────────────────────────────────────────────
TREE_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_tree)
TREE_TEST_OBJ		:= $(call TEST_OBJECT_PATH,red_black_tree)
TREE_TEST_BIN		:= $(call TEST_BINARY_PATH,red_black_tree)
# ─────────────── target prequisites ───────────────────────────────────────────
TREE_TEST_OBJ_PREQS	:= $(TREE_TEST_SRC)				\
			   $(TEST_HDR)					\
			   $(TREE_HDR)					\
			   $(INT_KEY_HDR)
TREE_TEST_BIN_PREQS	:= $(TREE_TEST_OBJ)				\
			   $(TEST_OBJ_GROUP)				\
			   $(INT_KEY_OBJ_GROUP)				\
			   $(TREE_SH_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(TREE_TEST_OBJ)				\
			   $(TREE_TEST_BIN)


# red_black_lhmap_test
# ──────────────────────────────────────────────────────────────────────────────
LHMAP_TEST_SRC		:= $(call TEST_SOURCE_PATH,red_black_lhmap)
LHMAP_TEST_OBJ		:= $(call TEST_OBJECT_PATH,red_black_lhmap)
LHMAP_TEST_BIN		:= $(call TEST_BINARY_PATH,red_black_lhmap)
# ─────────────── target prequisitTEST_es ───────────────────────────────────────────
LHMAP_TEST_OBJ_PREQS	:= $(LHMAP_TEST_SRC)				\
			   $(TEST_HDR)					\
			   $(LHMAP_HDR)
LHMAP_TEST_BIN_PREQS	:= $(LHMAP_TEST_OBJ)				\
			   $(TEST_OBJ_GROUP)				\
			   $(MT_TEST_OBJ_GROUP)				\
			   $(LHMAP_SH_LIB)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(LHMAP_TEST_OBJ)				\
			   $(LHMAP_TEST_BIN)



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

$(LHMAP_TEST_BIN): $(LHMAP_TEST_BIN_PREQS)
	$(LD) $^ $(THREADS_LIB) $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(LHMAP_TEST_OBJ): $(LHMAP_TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


$(TREE_INSERTION_TEST_BIN): $(TREE_INSERTION_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(TREE_INSERTION_TRNR_OBJ): $(TREE_INSERTION_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INSERTION_TEST_OBJ): $(TREE_INSERTION_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(TREE_INSERTION_TRNR_SRC): $(TREE_INSERTION_TRNR_SRC_PREQS)
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


$(CONCAT_TEST_BIN): $(CONCAT_TEST_BIN_PREQS)
	$(LD) $^ $(LD_LIBS) $(LD_FLAGS) $(LD_BIN_FLAGS) -o $@
$(CONCAT_TRNR_OBJ): $(CONCAT_TRNR_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(CONCAT_TEST_OBJ): $(CONCAT_TEST_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) $< -o $@
$(CONCAT_TRNR_SRC): $(CONCAT_TRNR_SRC_PREQS)
	$(RUBY) $(RUBY_FLAGS) $(UNITY_GEN_TRNR_SCRIPT) $< $@

$(MT_TEST_OBJ): $(MT_TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(TEST_OBJ): $(TEST_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


# RED_BLACK_LHMAP
# ──────────────────────────────────────────────────────────────────────────────
$(LHMAP_ST_LIB): $(LHMAP_ST_LIB_PREQS)
	$(AR) $(AR_FLAGS) $@ $^
$(LHMAP_SH_LIB): $(LHMAP_SH_LIB_PREQS)
	$(LD) $^ $(THREADS_LIB) $(LD_LIBS) $(LD_FLAGS) $(LD_SH_FLAGS) -o $@
$(LHMAP_OBJ): $(LHMAP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(LHMAP_PIC_OBJ): $(LHMAP_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@


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


# RED_BLACK MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(NODE_FACTORY_OBJ): $(NODE_FACTORY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(NODE_FACTORY_PIC_OBJ): $(NODE_FACTORY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(ETOR_OBJ): $(ETOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(ETOR_PIC_OBJ): $(ETOR_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(LINK_OFFSET_OBJ): $(LINK_OFFSET_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(LINK_OFFSET_PIC_OBJ): $(LINK_OFFSET_PIC_OBJ_PREQS)
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

$(CONCAT_OBJ): $(CONCAT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(CONCAT_PIC_OBJ): $(CONCAT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(COPY_OBJ): $(COPY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(COPY_PIC_OBJ): $(COPY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(CONGRUENT_OBJ): $(CONGRUENT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(CONGRUENT_PIC_OBJ): $(CONGRUENT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HKEY_OBJ): $(HKEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HKEY_PIC_OBJ): $(HKEY_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

$(HMAP_COUNT_OBJ): $(HMAP_COUNT_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(HMAP_COUNT_PIC_OBJ): $(HMAP_COUNT_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@

ifneq (T,$(SYSTEM_WINDOWS))
$(LOCK_OBJ): $(LOCK_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@
$(LOCK_PIC_OBJ): $(LOCK_PIC_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $(CC_PIC_FLAGS) $< -o $@
endif


# KEY_ACCESSOR MODULES
# ──────────────────────────────────────────────────────────────────────────────
$(INT_KEY_OBJ): $(INT_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@

$(STR_KEY_OBJ): $(STR_KEY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) $< -o $@


# UNITY OBJECT
# ──────────────────────────────────────────────────────────────────────────────
$(UNITY_OBJ): $(UNITY_OBJ_PREQS)
	$(CC) $(UNITY_CC_FLAGS) -c $< -o $@
