CC=gcc
CPPFLAGS=-Wall -Wextra

.PHONY: all
all: server-build journalist-build clean
test: server-test journalist-test clean


server-build:
	@make -C ./server

journalist-build:
	@make -C ./journalist

server-test:
	@make -C ./server test

journalist-test:
	@make -C ./journalist test

clean:
	@rm -f *.o
	@make -C ./server clean
	@make -C ./journalist clean

reset:
	@make -C ./server reset
	@make -C ./journalist reset