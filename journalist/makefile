CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all clean
all: main

test: server.o
	$(CC) -o plague-journalist-test.x $< test.c

main: server.o
	$(CC) -o plague-journalist.x $< main.c

server.o: $(SRCPATH)*.c $(SRCPATH)*.h
	$(CC) $(CPPFLAGS) $(SRCPATH)$*.c -c

clean:
	rm -f *.o

reset: clean
	rm -f *.x