CC=gcc
CPPFLAGS=-Wall -Wextra

main: matrix.o
	$(CC) -o programme matrix.o main.c
	make clean

matrix.o: matrix.c matrix.h
	$(CC) $(CPPFLAGS) $*.c -c

clean:
	rm -f *.o