CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all clean
all: main

main: server.o
	$(CC) -o plague-server.x $< main.c

server.o: $(SRCPATH)*.c $(SRCPATH)*.h
	$(CC) $(CPPFLAGS) $(SRCPATH)$*.c -c

clean:
	rm -f *.o

reset:
	rm -f *.x