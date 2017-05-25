#Macros
CC=gcc
CFLAGS=-Wall -g -c
GENNUM=100
POPNUM=100
LENGTH=16
SEED=

all: runMe

runMe: geneticAlgorithms.o
	$(CC) geneticAlgorithms.o -o runMe

geneticAlgorithms.o: geneticAlgorithms.c geneticAlgorithms.h
	$(CC) $(CFLAGS) geneticAlgorithms.c



# debugMe: debugMe.o
# 	$(CC) debugMe.o -o debugMe

# debugMe.o: geneticAlgorithms.c geneticAlgorithms.h
# 	$(CC) $(CFLAGS) -DDEBUG geneticAlgorithms.c -o debugMe.o

run: runMe
	./runMe $(GENNUM) $(POPNUM) $(LENGTH)

debug: debugMe
	./debugMe $(GENNUM) $(POPNUM) $(LENGTH)

valgrind: runMe
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes -v ./runMe $(GENNUM) $(POPNUM) $(LENGTH)

clean: 
	rm -rf runMe debugMe *.o