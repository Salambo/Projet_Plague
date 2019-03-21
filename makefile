CC=gcc
CPPFLAGS=-Wall -Wextra

.PHONY: all clean
all: server-build journalist-build


server-build:
	make -C ./server

journalist-build:
	make -C ./journalist

clean:
	rm -f *.o
	make -C ./server clean
	make -C ./journalist clean

reset:
	make -C ./server reset
	make -C ./journalist reset