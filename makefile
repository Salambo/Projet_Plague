CC=gcc
CPPFLAGS=-Wall -Wextra

server: matrix.o
	$(CC) -o programme matrix.o main.c
	make clean

journalist: 

server.o:

journalist.o: matrix.c matrix.h
	$(CC) $(CPPFLAGS) $*.c -c

clean:
	rm -f *.o