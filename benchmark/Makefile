KEYS_COUNT 	:= 15000000
CC	   	:= gcc
CPP		:= g++
CC_FLAGS	:= -std=c99
CPP_FLAGS	:= -std=c++11
OPT_FLAGS   	:= -O2 -funroll-loops
INC_FLAG	:= -I../include
TREE_LIB   	:= ../static/libred_black_tree.a
HMAP_LIB   	:= ../static/libred_black_hmap.a
SHARED_FLAGS    := $(OPT_FLAGS) -DKEYS_COUNT=$(KEYS_COUNT)

all: tree hmap unordered_set ordered_set

tree: tree.c
	$(CC) $(CC_FLAGS) $(SHARED_FLAGS) $(INC_FLAG) $< $(TREE_LIB) -o $@

hmap: hmap.c
	$(CC) $(CC_FLAGS) $(SHARED_FLAGS) $(INC_FLAG) $< $(HMAP_LIB) -o $@

unordered_set: unordered_set.cpp
	$(CPP) $(CPP_FLAGS) $(SHARED_FLAGS) $< -o $@

ordered_set: ordered_set.cpp
	$(CPP) $(CPP_FLAGS) $(SHARED_FLAGS) $< -o $@

clean:
	rm tree hmap unordered_set ordered_set
