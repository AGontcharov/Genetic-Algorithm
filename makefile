#Macros
CC=gcc
CFLAGS=-Wall -g -c
GENNUM=1000
POPNUM=100
LENGTH=16

all: runMe

runMe: geneticAlgorithms.o
	$(CC) geneticAlgorithms.o -o runMe

geneticAlgorithms.o: geneticAlgorithms.c geneticAlgorithms.h
ifeq ($(MAKECMDGOALS),debug)
	$(CC) $(CFLAGS) -DDEBUG geneticAlgorithms.c
else
	$(CC) $(CFLAGS) geneticAlgorithms.c
endif

run: runMe
	./runMe $(GENNUM) $(POPNUM) $(LENGTH) $(SEED)

debug: runMe
	./runMe $(GENNUM) $(POPNUM) $(LENGTH) $(SEED)

valgrind: runMe
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes -v ./runMe $(GENNUM) $(POPNUM) $(LENGTH) $(SEED)

clean: 
	rm -rf runMe *.o