CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all clean
all: main

test: citoyens.o server.o test.o
	$(CC) -o $@ $^ -lpthread -lrt

test.o: test.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c



main: citoyens.o server.o main.o
	$(CC) -o $@ $^ -lpthread -lrt

main.o: $(SRCPATH)server.h main.c
	@$(CC) -o $@ $< -c

citoyens.o: $(SRCPATH)citoyens.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c

server.o: $(SRCPATH)server.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c

#server.o: $(SRCPATH)*.c $(SRCPATH)*.h
#	$(CC) $(CPPFLAGS) $(SRCPATH)$*.c -c

clean:
	@rm -f *.o

reset: clean
	@rm -f *.x test main