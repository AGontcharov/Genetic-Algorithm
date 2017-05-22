#!/bin/bash

#Variable length test
echo "----------------------------" >> resultsLength.txt
echo "Test Suit #1 variable length" >> resultsLength.txt
echo "----------------------------" >> resultsLength.txt
for value in {1..3}
do
	echo "Gen: 1000 Pop: 50 Length: 8" >> resultsLength.txt
	sleep 1
	./runMe 1000 50 8 >> resultsLength.txt
done

for value in {1..3}
do
	echo "Gen: 1000 Pop: 50 Length: 16" >> resultsLength.txt
	./runMe 1000 50 16 >> resultsLength.txt
done

for value in {1..3}
do
	echo "Gen: 1000 Pop: 50 Length: 32" >> resultsLength.txt
	./runMe 1000 50 32 >> resultsLength.txt
done

#Generation test
echo "----------------------------" >> resultsGen.txt
echo "Test Suit #2 Generation" >> resultsGen.txt
echo "----------------------------" >> resultsGen.txt
for value in {1..3}
do
	echo "Gen: 100 Pop: 50 Length: 16" >> resultsGen.txt
	./runMe 100 50 16 >> resultsGen.txt
done

for value in {1..3}
do
	echo "Gen: 500 Pop: 50 Length: 16" >> resultsGen.txt
	./runMe 500 50 16 >> resultsGen.txt
done

for value in {1..3}
do
	echo "Gen: 1000 Pop: 50 Length: 16" >> resultsGen.txt
	./runMe 1000 50 16 >> resultsGen.txt
done

#Population test
echo "----------------------------" >> resultsPop.txt
echo "Test Suit #3 Populatlion Size" >> resultsPop.txt
echo "----------------------------" >> resultsPop.txt
for value in {1..3}
do
	echo "Gen: 1000 Pop: 25 Length: 16" >> resultsPop.txt
	./runMe 1000 25 16 >> resultsPop.txt
done

for value in {1..3}
do
	echo "Gen: 1000 Pop: 50 Length: 16" >> resultsPop.txt
	./runMe 1000 50 16 >> resultsPop.txt
done

for value in {1..3}
do
	echo "Gen: 1000 Pop: 100 Length: 16" >> resultsPop.txt
	./runMe 1000 100 16 >> resultsPop.txt
done