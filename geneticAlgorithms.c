#include "geneticAlgorithms.h"

int main(int argc, char * argv[]) {	
	int maxGenerationNum = 0; 				// The maximum generation
	int generationPopulation = 0;			// Size of the population of the current generation
	char ** bitStrings = NULL;				// The default population
	int bitLength = 0;						// Length of the bit string
	int i = 0;

	time_t t;


	/* Command line argument defensive checking. */
	if (argc != 4) {
		fprintf(stderr, "Missing argument(s): [Max generation number] [Population size] [Bit string length]\n");
		exit(-1);
	}

	/* Get the maximun generation and size of the population from command line arguments. */
	sscanf(argv[1], "%d", &maxGenerationNum);
	sscanf(argv[2], "%d", &generationPopulation);
	sscanf(argv[3], "%d", &bitLength);

	/* Initialize the random number generator */ 
	//srand(time(NULL));

	srand((unsigned) time(&t));

	/* Create and initailize the bitstring array. */
	bitStrings = malloc(sizeof(char *) * generationPopulation);

	/* Bit strengh value hard coded for now. CHANGE*/
	for (i = 0; i < generationPopulation; i++) {
		bitStrings[i] = malloc(sizeof(char) * (bitLength + 7)/8);
		memset(bitStrings[i], 0, (bitLength + 7)/8);
	}	

	initPopulation(bitStrings, generationPopulation, bitLength);

	#if DEBUG
	printf("Initial population is: \n");
	printBitString(bitStrings, generationPopulation, bitLength);
	#endif

	// Begin the battle royale
	simulation(bitStrings, generationPopulation, maxGenerationNum, bitLength);

	for (i = 0; i < generationPopulation; i++) {
		free(bitStrings[i]);
	}
	free(bitStrings);

	return 0;
}

void initPopulation(char ** bitstring, int generationPopulation, int length) {
	int i = 0, j = 0, k = 0;

	/* Go through each string in the array.*/
	for (i = 0; i < generationPopulation; i++) {

		/* Go through each char in each string*/
		for (j = 0; j < (length + 7)/8; j++) {

			/* Go through each bit in char .*/
			for (k = 0; k < 8; k++)
			bitstring[i][j] |= rand() % 2 << k;
		}
	}
}

int setBitsFitness(char * a, int length) {
	unsigned int c = 0; 	//  count the number of bits set in each char
	unsigned int totalSetBits = 0;
	int i = 0;

	for (i = 0; i < (length + 7)/8; i++) {
		for (c = 0; a[i]; c++) {
		  	a[i] &= a[i] - 1; // clear the least significant bit set
		}
		totalSetBits += c;
	}

	//printf("Fitness: %d\n", totalSetBits);
	return totalSetBits;
}

int integerValueFitness(char * a, int length) {
	int value = 0;
	//int i = 0, j = 0;
	
	/* TEST*/
	/*printf("bit string is:");
	for (i = 0; i < (length + 7)/8; i++) {
		for (j = 0; j < 8; j++) {
      		printf("%d", !!((a[i] << j) & 0x80)); //understand what this does	
		}
  	}
	printf("\n");*/


	/* Covert to int */
	//value = *(unsigned int *)a;
	value = *(int *)a;
	//value = a[0] | ( (int)a[1] << 8 ) | ( (int)a[2] << 16 ) | ( (int)a[3] << 24 );
	//printf("value is: %d\n", value);

	/*if (value > 1073741823) {
		fprintf(stderr, "You've broken an int of 30 bits\n");
		exit(-2);
	}*/

	return value;
}

void bubbleSort(char ** parentBitString, int * populationFitness, int currentPopulation) {
	int i = 0, j = 0;
	int temp = 0;
	char * tempString = NULL;
	bool swaped = false;

	/* Print the unsorted array. */
	#if DEBUG
	for (i = 0; i < currentPopulation; i++) {
		printf("%d ", (populationFitness)[i]);
	}
	printf("\n");
	#endif

	/* In hindsight would be better to store bit strings and its fitness values in a struct. */
	for (i = 0; i < currentPopulation - 1; i++) {
		for (j = 0; j < currentPopulation - 1 - i; j++) {

			if (populationFitness[j] > populationFitness[j + 1]) {
				temp = populationFitness[j];
				populationFitness[j] = populationFitness[j + 1];
				populationFitness[j + 1] = temp;

				/* Sort the bit strings as well. */
				tempString = parentBitString[j];
				parentBitString[j] = parentBitString[j + 1];
				parentBitString[j + 1] = tempString;

				swaped = true;
			}
		}
		if (!swaped) break;
	}

	/* Print the sorted array. */
	#if DEBUG
	for (i = 0; i < currentPopulation; i++) {
		printf("%d ", populationFitness[i]);		
	}
	printf("\n");
	printBitString(parentBitString, currentPopulation, 16);
	#endif
}

int accumulatedFitness(double * weightedFitness, int * populationFitness, int currentPopulation) {
	int i = 0, j = 0;
	int accumulatedFitness = 0;

	for (i = 0; i < currentPopulation; i++) {
		accumulatedFitness = 0;
		
		/* Get the accumulated fitness for an individual. */
		for (j = i; j >= 0; j--) {
			accumulatedFitness += populationFitness[j];
		}
		weightedFitness[i] = accumulatedFitness;
	}

	/* Print the accumulated fitness values. */
	#if DEBUG
	for (i = 0; i < currentPopulation; i++) {
		printf("%d ", (int)weightedFitness[i]);
	}
	printf("\n");
	#endif

	/* Divide accumulate fitness by total fitness sum. */
	for (i = 0; i < currentPopulation; i++) {
		weightedFitness[i] = (weightedFitness[i] / weightedFitness[currentPopulation - 1]);
	}

	/* Print the standardized accumulated fitness values. */
	#if DEBUG
	for (i = 0; i < currentPopulation; i++) {
		printf("%lf ", weightedFitness[i]);
	}
	printf("\n");
	#endif

	return accumulatedFitness;
}

int selection(double * weightedFitness, int currentPopulation) {
	int i = 0;
	double selected = 0;

	selected = (double)rand()/(double)(RAND_MAX/1);

	#if DEBUG
	printf("Randon number is: %lf\n", selected);
	#endif

	for (i = 0; i < currentPopulation; i++) {

		if (selected < weightedFitness[i]) {
			break;
		}
	}

	/* Prevent the -1 index case. Not sure if I should regerage the random number though. */
	if ((i - 1) < 0) i = 1;
	
	/*What does it select if the randon number is lower than everything?*/
	#if DEBUG
	printf("Selected: %lf at index %d\n", weightedFitness[i - 1], i - 1);
	#endif

	return i - 1;
}

void crossover(char * destination, char * a, char * b, int length) {
	int split = 0;
	int i = 0;
	int bit = 0;
	char * newBitString = NULL;

	#if DEBUG
	int j = 0;
	#endif

	newBitString = malloc(sizeof(char) * (length + 7)/8);
	memset(newBitString, 0, (length + 7)/8);

	split = (rand() % (length - 1)) + 1;

	#if DEBUG
	printf("Performing cross over!\n");
	printf("Splitting at: %d\n", split);
	#endif 

	for (i = length - 1; i > split; i--) {
		bit = ((a[i/8]) >> (i % 8) & 1);

		#if DEBUG
		printf("a[%d] >> %d = %d\n", i/8, i % 8, bit);
		#endif

		newBitString[i/8] |= bit << (i % 8);
	}
	//printf("\n");

	for (i = split; i >= 0; i--) {
		bit = ((b[i/8]) >> (i % 8) & 1);
		
		#if DEBUG
		printf("b[%d] >> %d = %d\n", i/8, i % 8, bit);
		#endif

		newBitString[i/8] |= bit << (i % 8);
	}
	//printf("\n");

	strncpy(destination, newBitString, (length + 7)/8);
	free(newBitString);

	/* After cross over. */
	#if DEBUG
	for (i = 0; i < (length + 7)/8; i++) {
		for (j = 0; j < 8; j++) {
      		printf("%d", !!((newBitString[i] << j) & 0x80)); //understand what this does	
		}
  	}
	printf("\n");
	#endif

	//printf("\n");
}

void mutation(char * bitString, int length) {
	int position = 0;
	double chance = 0;
	//int i = 0, j = 0;

	#if DEBUG
	int i = 0;
	#endif

	// Roll the die for RNGESUS
	chance = (double)rand()/(double)(RAND_MAX/1);

	#if DEBUG
	printf("Chance for mutation is: %lf\n", chance);
	#endif

	/* Perform mutation. */
	if (chance < (double) 1/length) {

		/* Choose a random bit to flip. */
		position = rand() % length - 1;

		#if DEBUG
		printf("Mutating bit at position %d\n", position);
		#endif

		/* Before muration. */
		#if DEBUG
		for (i = 0; i < 8; i++) {
		    printf("%d", !!((*bitString << i) & 0x80)); //understand what this does
		}
		printf("\n");
		#endif

		/* Toggle bit at position */
		bitString[position/8] ^= 1 << position % 8;

		/* TEST*/
		/*printf("After mutation:");
		for (i = 0; i < (length + 7)/8; i++) {
			for (j = 0; j < 8; j++) {
	      		printf("%d", !!((bitString[i] << j) & 0x80)); //understand what this does	
			}
	  	}
		printf("\n");*/

		/* After mutation. */
		#if DEBUG
		for (i = 0; i < 8; i++) {
		    printf("%d", !!((*bitString << i) & 0x80)); //understand what this does
		}
		printf("\n");
		#endif
	}
}

void simulation(char ** parentBitString, int currentPopulation, int maxGenerationNum, int length) {
	int i = 0;
	int generationNum = 0; 					// The current generation
	int childrenPopulation = 0;				// Size of the children population
	int maxFitness = 8;						// Desirable fitness value
	int seenFitness = 0;					// The highest seen fitness value
	int generationFitness = 0;				// The fitness of the generation
	int * populationFitness = NULL;			// The fitness values of the bit strings
	double * weightedFitness = NULL;		// The weighted roulette of the bit strings fitness
	char * katniss = NULL;
	char * peeetah = NULL;
	char * temp = NULL;						// Temporary string to hold contents of parent bit strings.
	char ** childrenBitString = NULL;		// The children generation bit strings
	
	#if DEBUG
	int j = 0;
	#endif

	/* Create arrays to store fitness values and bit strings. */
	populationFitness = malloc(sizeof(int) * currentPopulation);
	weightedFitness = malloc(sizeof(double) * currentPopulation);
	katniss = malloc(sizeof(char) * length);
	peeetah = malloc(sizeof(char) * length);
	temp = malloc(sizeof(char) * length);

	childrenBitString = malloc(sizeof(char *) * currentPopulation);

	for (i = 0; i < currentPopulation; i++) {
		childrenBitString[i] = malloc(sizeof(char) * (length + 7)/8);
	}

	//Terminate on max fitness value too.
	while (generationNum < maxGenerationNum && seenFitness != maxFitness) {

		/* Initialize arrays. */
		memset(populationFitness, 0, currentPopulation);
		memset(weightedFitness, 0, currentPopulation);
	
		for (i = 0; i < currentPopulation; i++) {
			memset(childrenBitString[i], 0, (length + 7)/8);
		}

		memset(katniss, 0, (length + 7)/8);
		memset(peeetah, 0, (length + 7)/8);
		memset(temp, 0, (length + 7)/8);

		/* Calculate the fitness for each individual in the current generation. */
		for (i = 0; i < currentPopulation; i++) {
			strncpy(temp, parentBitString[i], (length + 7)/8);
			populationFitness[i] = setBitsFitness(temp, length);
			//populationFitness[i] = integerValueFitness(temp, length);
		}

		/* Sort the parentBitString and the populationFitness. */
		bubbleSort(parentBitString, populationFitness, currentPopulation);

		/* Highest fitness value seen so far. */
		if (seenFitness < populationFitness[currentPopulation - 1]) {
			seenFitness = populationFitness[currentPopulation - 1];
		}

		/* Get the accumalated standardized fitness for each individual and generation fitness. */
		generationFitness = accumulatedFitness(weightedFitness, populationFitness, currentPopulation);
		
		if (generationNum == 0) {
			printf("Starting generation!\n");
			printf("GENERATION %d: \n", generationNum);
			printf("Highest seen fitness is: %d\n", seenFitness);
			printf("Number of bits set(1): %d\n\n", generationFitness);	
		}
		
		#if DEBUG
		printBitString(parentBitString, currentPopulation, length);
		#endif

		while (childrenPopulation < currentPopulation) {

			/* Perform selection .*/
			strncpy(katniss, parentBitString[selection(weightedFitness, currentPopulation)], (length + 7)/8);

			/* Print katniss */
			#if DEBUG
			for (i = 0; i < (length + 7)/8; i++) {
				for (j = 0; j < 8; j++) {
		      		printf("%d", !!((katniss[i] << j) & 0x80)); //understand what this does	
				}
		  	}
	 		printf("\n");
		  	#endif

			strncpy(peeetah, parentBitString[selection(weightedFitness, currentPopulation)], (length + 7)/8);

		  	/* Print peeetah. */
		  	#if DEBUG
		  	for (i = 0; i < (length + 7)/8; i++) {
				for (j = 0; j < 8; j++) {
		      		printf("%d", !!((peeetah[i] << j) & 0x80)); //understand what this does	
				}
		  	}
	 		printf("\n");
		  	#endif

			/* Perform crossover and mutation. */
		  	crossover(childrenBitString[childrenPopulation], katniss, peeetah, length);
		  	mutation(childrenBitString[childrenPopulation], length);

			/* Sanity check. */
			#if DEBUG
			for (i = 0; i < 8; i++) {
			    printf("%d", !!((childrenBitString[childrenPopulation] << i) & 0x80)); //understand what this does
			}
			printf("\n");
			#endif

			childrenPopulation++;
		}

		#if DEBUG
		printf("Done making children!\n");
		printBitString(childrenBitString, childrenPopulation, length);
		#endif

		/* Children become the parents now for the next generation. */

		for (i = 0; i < childrenPopulation; i++) {
			strncpy(parentBitString[i], childrenBitString[i], (length +7)/8);
		}
		
		currentPopulation = childrenPopulation;
		childrenPopulation = 0;
		generationNum++;

		#if DEBUG
		printf("Parents now are: \n");
		printBitString(parentBitString, currentPopulation, length);
		#endif
	}

	printf("Solution found or maximum generation reached!\n");
	printf("GENERATION %d: \n", generationNum);
	printf("Highest seen fitness is: %d\n", seenFitness);
	printf("Number of bits set(1): %d\n\n", generationFitness);

	/* Free all the allocated arrays. */
	for (i = 0; i < currentPopulation; i++) {
		free(childrenBitString[i]);
	}
	free(childrenBitString);
	free(populationFitness);
	free(weightedFitness);
	free(katniss);
	free(peeetah);
	free(temp);
}

void printBitString(char ** bitstring, int generationPopulation, int length) {
	int i = 0, j = 0, k = 0;
  	
  	/* Go through each string in the array. */
	for (i = 0; i < generationPopulation; i++) {

		/* Go through each character in the string.*/
		for (j = 0; j < (length + 7)/8; j++) {

			for (k = 0; k < 8; k++) {
	      		printf("%d", !!((bitstring[i][j] << k) & 0x80)); //understand what this does	
			}
	  	}
 		printf("\n");
	} 
}