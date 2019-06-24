SRCS:=$(wildcard *.c)
#OBJS:=$(SRCS:%.c=%.o)
ELFS:=$(SRCS:%.c=%)
all:$(ELFS)
	@for i in $(ELFS); do gcc -o $${i} $${i}.c; done
clean:
	rm -rf $(ELFS)
