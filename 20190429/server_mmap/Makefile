SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:%.c=%.o)
ELF:=server_mmap
CC:=gcc
CFLAGS:=-g -Wall -D DEBUG
$(ELF): $(OBJS)
	gcc $^ -o $@
.PHONY: clean
clean:
	rm -rf $(OBJS) $(ELF)
