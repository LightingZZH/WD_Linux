SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:%.c=%.o)
ELF:=process_poll_server
CC:=gcc
CFLAGS:=-g -Wall -D DEBUG
$(ELF): $(OBJS)
	gcc $^ -o $@
.PHONY: clean
clean:
	rm -rf $(OBJS) $(ELF)
