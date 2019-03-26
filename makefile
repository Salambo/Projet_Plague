CC=gcc
CPPFLAGS=-Wall -Wextra

.PHONY: all clean
all: server-build journalist-build
test: server-test journalist-test


server-build:
	make -C ./server

journalist-build:
	make -C ./journalist

server-test:
	make -C ./server test

journalist-test:
	make -C ./journalist test

clean:
	rm -f *.o
	make -C ./server clean
	make -C ./journalist clean

reset:
	make -C ./server reset
	make -C ./journalist reset