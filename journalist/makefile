CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all
all: main clean

test: journalist.o test.o
	@$(CC) -o $@ $^ -lpthread -lrt

test.o: test.c $(SRCPATH)journalist.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)



main: journalist.o main.o
	@$(CC) -o $@ $^ -lpthread -lrt

main.o: main.c $(SRCPATH)journalist.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)

journalist.o: $(SRCPATH)journalist.c $(SRCPATH)journalist.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)

clean:
	@rm -f *.o

reset: clean
	@rm -f *.x test main