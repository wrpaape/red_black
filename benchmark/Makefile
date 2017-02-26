KEYS_COUNT 	:= 10000000
CC	   	:= gcc
CPP		:= g++
OP_FLAGS   	:= -O2 -funroll-loops
INC_FLAG	:= -I../include
TREE_LIB   	:= ../static/libred_black_tree.a
HMAP_LIB   	:= ../static/libred_black_hmap.a
SHARED_FLAGS    := $(OP_FLAGS) -DKEYS_COUNT=$(KEYS_COUNT)

all: tree hmap unordered_set ordered_set

tree: tree.c
	$(CC) $(SHARED_FLAGS) $(INC_FLAG) $< $(TREE_LIB) -o $@

hmap: hmap.c
	$(CC) $(SHARED_FLAGS) $(INC_FLAG) $< $(HMAP_LIB) -o $@

unordered_set: unordered_set.cpp
	$(CPP) $(SHARED_FLAGS) $< -o $@

ordered_set: ordered_set.cpp
	$(CPP) $(SHARED_FLAGS) $< -o $@

clean:
	rm tree hmap unordered_set ordered_set
