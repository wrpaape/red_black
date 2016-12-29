CC		:= gcc
CC_FLAGS	:= -std=gnu99 -I. -Wall -O2 -c
LD		:= ld
LD_FLAGS	:= -arch x86_64 -macosx_version_min 10.11 -no_pie
RM		:= rm
RM_FLAGS	:= -rf


all: red_black

red_black: key_utils.o red_black_print.o red_black_verify.o red_black_find.o red_black_delete.o red_black_insert.o red_black.o
	$(LD) $(LD_FLAGS) $^ -lc -o $@

red_black.o: red_black.c red_black.h red_black_insert.h red_black_delete.h red_black_find.h red_black_print.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_insert.o: red_black_insert.c red_black_insert.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_delete.o: red_black_delete.c red_black_delete.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_find.o: red_black_find.c red_black_find.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_verify.o: red_black_verify.c red_black_verify.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

red_black_print.o: red_black_print.c red_black_print.h red_black_utils.h key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

key_utils.o: key_utils.c key_utils.h
	$(CC) $(CC_FLAGS) $< -o $@

clean:
	$(RM) $(RM_FLAGS) red_black red_black.o red_black_insert.o red_black_delete.o red_black_find.o red_black_verify.o  red_black_print.o key_utils.o
