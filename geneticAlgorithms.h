#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while(0)
#endif

/* Creates the bit strings for a generation based on the population size and initializes them with random values.
 * bitStrings: The starting generation bit strings 
 * generationPopulation: The population size of the generation
 * length: Length of the bit strings
 * Return: N/a */
void initPopulation(char ** bitStrings, int generationPopulation, int length);

/* Calculates the fitness value for a given bit string based on the number of set bits. Implemented using  Brian Kernighan’s Algorithm.
 * a: A bit string
 * length: Length of the bit strings
 * Return: The fitness value of the bit string */
int setBitsFitness(char * a, int length);

/* Sorts the generation bit strings and its fitness value in ascending order through the bubble sort algorithm.
 * parentBitStrings: The generation bit strings
 * populationfFitness: The fitness of each individual in the generation
 * currentPopulation: The population size of the generation
 * Return: N/a */
void bubbleSort(char ** parentBitStrings, int * populationFitness, int currentPopulation);

/* Calculates the standardized fitness values for each individual in a generation.
 * weightedFitness: The standardized fitness values for a generation
 * populationfFitness: The fitness of each individual in the generation
 * currentPopulation: The population size of the current generation
 * Return: The maximum fitness accumulated */
int accumulatedFitness(double * weightedFitness, int * populationFitness, int currentPopulation);

/* Selects an individual based on theit fitness level through the weighted roulette selection method.
 * weightedFitness: The fitness of each individual in the generation 
 * currentPopulation: The population size of the current generation
 * Return: The index of the individual selected */
int selection(double * weightedFitness, int currentPopulation);

/* Performs single point cross over between two bit strings and spawns offsprings.
 * childrenBitStrings: The children generation
 * index: Location to insert new children in children population
 * maxPopulation: The maximum size of the generation
 * a: Bit string pair to cross over with
 * b: Bit string pair to cross over with
 * length: The length of the bit strings
 * Return: The Amount of children added to the children generation */
int crossover(char ** childrenBitStrings, int index, int maxPopulation, char * a, char * b, int length);

/* Start the genetic algorithm simulation and search fot the solution or until terminating generation has been reached.
 * parentBitStrings: The generation bit strings 
 * populationfFitness: The fitness of each individual in the generation
 * maxGenerationNum: The terminating generation
 * length: The length of the bit strings
 * Return: N/a */
void simulation(char ** parentBitStrings, int currentPopulation, int maxGenerationNum, int length);

/* Prints all the bit strings for a given generation and its population size.
 * bitString: The generation bit strings 
 * generationPopulation: The population size of the generation
 * length: The length of the bit strings
 * Return: N/a */
void printBitStrings(char ** bitStrings, int generationPopulation, int length);

/* Very low chance (1/length) to perform mutation on a bit string.
 * bitString: The bit string to randomly toggle a bit.
 * length: The length of the bit strings
 * Return: N/a */
void mutation(char * bitString, int length);