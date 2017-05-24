#Macros
CC=gcc
CFLAGS=-Wall -g -c
GENNUM=1000
POPNUM=100
LENGTH=32

all: runMe

runMe: geneticAlgorithms.o
	$(CC) geneticAlgorithms.o -o runMe

geneticAlgorithms.o: geneticAlgorithms.c geneticAlgorithms.h
	$(CC) $(CFLAGS) geneticAlgorithms.c

run: runMe
	./runMe $(GENNUM) $(POPNUM) $(LENGTH)

test: test.sh runMe
	./test.sh

#Need -DDEBUG=1

#debug: debugMe
#	./debugMe $(GENNUM) $(POPNUM)

#debugMe: geneticAlgorithms.o
#	$(CC) geneticAlgorithms.o -o debugMe

valgrind: runMe
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes -v ./runMe $(GENNUM) $(POPNUM) $(LENGTH)

clean: 
	rm runMe *.o