CC=gcc
CPPFLAGS=-Wall -Wextra
SRCPATH=src/

.PHONY: all
all: main clean

test: citoyens.o server.o test.o
	$(CC) -o $@ $^ -lpthread -lrt

test.o: test.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)



main: citoyens.o server.o main.o
	$(CC) -o $@ $^ -lpthread -lrt

main.o: main.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)

citoyens.o: $(SRCPATH)citoyens.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)

server.o: $(SRCPATH)server.c $(SRCPATH)server.h
	@$(CC) -o $@ $< -c $(CPPFLAGS)

#server.o: $(SRCPATH)*.c $(SRCPATH)*.h
#	$(CC) $(CPPFLAGS) $(SRCPATH)$*.c -c

clean:
	@rm -f *.o

reset: clean
	@rm -f *.x test main