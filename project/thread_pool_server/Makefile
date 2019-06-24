#con_mysql:Mysql.c interactive.c
#	gcc -c Mysql.c -I /usr/include/mysql/ -lmysqlclient
#	gcc -c interactive.c -I /usr/include/mysql/ -lmysqlclient
SRCS:=$(wildcard *.c)
OBJS:= $(SRCS:%.c=%.o)
ELF:=server
#CC:=gcc
CFLAGS:=-I /usr/include/mysql/
$(ELF):$(OBJS)
	gcc $^ -o $@ -pthread -lmysqlclient
clean:
	rm -rf $(ELF) $(OBJS)
