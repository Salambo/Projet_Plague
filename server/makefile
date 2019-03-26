CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all clean
all: main

test: server.o
	$(CC) -o plague-server-test.x $< test.c

main: server.o
	$(CC) -o plague-server.x $< main.c

server.o: $(SRCPATH)*.c $(SRCPATH)*.h
	$(CC) $(CPPFLAGS) $(SRCPATH)$*.c -c

clean:
	rm -f *.o

reset: clean
	rm -f *.x