all: httpd client
LIBS = -lpthread #-lsocket
httpd: httpd.c
	gcc $(LIBS) -o $@ $<

client: simpleclient.c
	gcc -W -Wall -o $@ $<
clean:
	rm httpd
