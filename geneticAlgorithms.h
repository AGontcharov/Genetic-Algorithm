#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define DEBUG 0

/* Creates the bit strings for a generation based on the population size and initializes them with random values.
 * bitString: The generation bit strings 
 * generationPopulation: The population size of the generation
 * Return: The generation bit string */
void initPopulation(char ** bitString, int generationPopulation, int length);

/* Calculates the fitness value for a given bit string based on the number of set bits.
 * a: A bit string
 * Return: The fitness value of the bit string. */
int setBitsFitness(char * a, int length);

/* Calcualte the fitness value for a given bit string based on the maximum integer value.
 * a: A bit string
 * Return:  */
int integerValueFitness(char * a, int length);

/* Sorts the generation bit strings and its fitness value in ascending order through the bubble sort algorithm.
 * bitString: The generation bit strings 
 * populationfFitness: The fitness of each individual in the generation
 * currentPopulation: The population size of the generation
 * Return: N/a */
void bubbleSort(char ** parentBitString, int * populationFitness, int currentPopulation);

/* Calculates the standardized fitness values for each individual in a generation.
 * weightedFitness: The array to store the standardized fitness values for a generation
 * populationfFitness: The fitness of each individual in the generation
 * currentPopulation: The population size of the current generation
 * Return: The maximum fitness accumulated */
int accumulatedFitness(double * weightedFitness, int * populationFitness, int currentPopulation);

/* Selects an individual based on theit fitness level through the weighted roulette selection method.
 * populationfFitness: The fitness of each individual in the generation 
 * currentPopulation: The population size of the current generation
 * Return: The index of the individual selected */
int selection(double * weightedFitness, int currentPopulation);

/* Performs single point cross over between 2 bit strings and spawns an offspring.
 * a: Bit string pair to cross over with
 * b: Bit string pair to cross over with
 * Return: The newly produced bit string from the cross over with a and b. */
void crossover(char * destination, char * a, char * b, int length);

/* Start the genetic algorithm simulation and search fot the solution or until terminating generation has been reached.
 * bitString: The generation bit strings 
 * populationfFitness: The fitness of each individual in the generation
 * maxGenerationNum: The terminating generation
 * Return: N/a */
void simulation(char ** parentBitString, int currentPopulation, int maxGenerationNum, int length);

/* Prints all the bit strings for a given generation and its population size.
 * bitString: The generation bit strings 
 * generationPopulation: The population size of the generation
 * Return: N/a */
void printBitString(char ** bitstring, int generationPopulation, int length);

/* Very low chance to perform mutation on a bit string.
 * bitString: The bit string to randomly toggle a bit.
 * Return: N/a */
void mutation(char * bitString, int length);