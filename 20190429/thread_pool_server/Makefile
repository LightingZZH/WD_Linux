SRCS:=$(wildcard *.c)
OBJS:= $(SRCS:%.c=%.o)
ELF:=server
$(ELF):$(OBJS)
	gcc $^ -o $@ -pthread
clean:
	rm -rf $(ELF) $(OBJS)
