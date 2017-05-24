#include "geneticAlgorithms.h"

int main(int argc, char * argv[]) { 
    int maxGenerationNum = 0;               // The maximum generation
    int generationPopulation = 0;           // Size of the population of the current generation
    char ** bitStrings = NULL;              // The default population
    int bitLength = 0;                      // Length of the bit string
    int seed = 0;                           // Seed of the random number generator
    int i = 0;

    /* Command line argument defensive checking. */
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Missing argument(s): [Max generation number] [Population size] [Bit string length] [optinal: seed]\n");
        exit(-1);
    }

    /* Get the maximun generation and size of the population from command line arguments. */
    sscanf(argv[1], "%d", &maxGenerationNum);
    sscanf(argv[2], "%d", &generationPopulation);
    sscanf(argv[3], "%d", &bitLength);

    /* Get the seed if given. */
    if (argc == 5) sscanf(argv[4], "%d", &seed);

    /* Initialize the random number generator */ 
    seed = time(NULL);
    srand(seed);

    /* Create and and populate all the bit strings with random values. */
    bitStrings = malloc(sizeof(char *) * generationPopulation);
    for (i = 0; i < generationPopulation; i++)
        bitStrings[i] = calloc((bitLength + 7)/8, sizeof(char));   
    initPopulation(bitStrings, generationPopulation, bitLength);

    #if DEBUG
    printf("Initial population is: \n");
    printBitStrings(bitStrings, generationPopulation, bitLength);
    #endif
    
    printf("\nStarting generation!\n");
    printf("Seed: %u\n", (unsigned)seed);

    /* Begin the battle royal. */
    simulation(bitStrings, generationPopulation, maxGenerationNum, bitLength);

    for (i = 0; i < generationPopulation; i++) {
        free(bitStrings[i]);
    }
    free(bitStrings);

    return 0;
}

void initPopulation(char ** bitStrings, int generationPopulation, int length) {
    int i = 0, j = 0, k = 0;

    /* Go through each string in the array. */
    for (i = 0; i < generationPopulation; i++) {

        /* Go through each char in each string. */
        for (j = 0; j < (length + 7)/8; j++) {

            /* Go through each bit in char and set it based to the randon number.*/
            for (k = 0; k < 8; k++)
                bitStrings[i][j] |= rand() % 2 << k;
        }
    }
}

int setBitsFitness(char * a, int length) {
    unsigned int c = 0;                     // Count the number of bits set in each char
    unsigned int totalSetBits = 0;          // Count the total number of bits set in string
    int i = 0;

    for (i = 0; i < (length + 7)/8; i++) {

        /* While bit string is not 0. */
        for (c = 0; a[i]; c++) {
            a[i] &= a[i] - 1;               // Clear the least significant bit set and assign to bit string
        }
        totalSetBits += c;
    }
    return totalSetBits;
}

void bubbleSort(char ** parentBitStrings, int * populationFitness, int currentPopulation) {
    int i = 0, j = 0, temp = 0;
    char * tempString = NULL;
    bool swaped = false;

    /* Print the unsorted array. */
    #if DEBUG
    for (i = 0; i < currentPopulation; i++) printf("%d ", (populationFitness)[i]);
    printf("\n");
    #endif

    /* In hindsight would be better to store bit strings and its fitness values in a struct. */
    for (i = 0; i < currentPopulation - 1; i++) {
        for (j = 0; j < currentPopulation - 1 - i; j++) {

            /* Sort the population fitness and the bit strings. */
            if (populationFitness[j] > populationFitness[j + 1]) {
                temp = populationFitness[j];
                populationFitness[j] = populationFitness[j + 1];
                populationFitness[j + 1] = temp;

                tempString = parentBitStrings[j];
                parentBitStrings[j] = parentBitStrings[j + 1];
                parentBitStrings[j + 1] = tempString;
                swaped = true;
            }
        }
        if (!swaped) break;
    }

    /* Print the sorted array. */
    #if DEBUG
    for (i = 0; i < currentPopulation; i++) printf("%d ", populationFitness[i]);        
    printf("\n");
    printBitStrings(parentBitStrings, currentPopulation, 16);
    #endif
}

int accumulatedFitness(double * weightedFitness, int * populationFitness, int currentPopulation) {
    int i = 0, j = 0;
    int accumulatedFitness = 0;

    for (i = 0; i < currentPopulation; i++) {
        accumulatedFitness = 0;
        
        /* Get the accumulated fitness for an individual. */
        for (j = i; j >= 0; j--)
            accumulatedFitness += populationFitness[j];

        weightedFitness[i] = accumulatedFitness;
    }

    /* Print the accumulated fitness values. */
    #if DEBUG
    for (i = 0; i < currentPopulation; i++) printf("%d ", (int)weightedFitness[i]);
    printf("\n");
    #endif

    /* Divide accumulate fitness by total fitness sum. */
    for (i = 0; i < currentPopulation; i++)
        weightedFitness[i] = (weightedFitness[i] / weightedFitness[currentPopulation - 1]);

    /* Print the standardized accumulated fitness values. */
    #if DEBUG
    for (i = 0; i < currentPopulation; i++) printf("%lf ", weightedFitness[i]);
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

    for (i = currentPopulation - 1; i >= 0; i--)
        if (weightedFitness[i] < selected || i == 0) break;

    #if DEBUG
    printf("Selected: %lf at index %d\n", weightedFitness[i], i);
    #endif

    return i;
}

int crossover(char ** childrenBitStrings, int index, int maxPopulation, char * a, char * b, int length) {
    int split = 0, bit = 0, added = 0, i = 0;
    char * offspringC = NULL;                   // Child one from crossover
    char * offspringD = NULL;                   // Child two from crossover

    #if DEBUG
    int j = 0;
    #endif

    offspringC = calloc((length + 7)/8, sizeof(char));
    offspringD = calloc((length + 7)/8, sizeof(char));
    split = (rand() % (length - 1)) + 1;

    #if DEBUG
    printf("Performing cross over!\n");
    printf("Splitting at: %d\n", split);
    #endif 

    for (i = 0; i < split; i++) {

        /* Child C .*/
        bit = !!((a[i/8] << i % 8) & 0x80);
        offspringC[i/8] |= bit << (7 - (i % 8));
        //printf("%d a[%d] << %d == %d\n", i, i/8, 7 - (i % 8), bit);

        /* Child D .*/
        bit = !!((b[i/8] << i % 8) & 0x80);
        offspringD[i/8] |= bit << (7 - (i % 8));
        //printf("%d b[%d] << %d == %d\n", i, i/8, 7 - (i % 8), bit);
    }
    //printf("\n");

    for (i = split; i < length; i++) {

        /* Child C. */
        bit = !!((b[i/8] << i % 8) & 0x80);
        offspringC[i/8] |= bit << (7 - (i % 8));
        //printf("%d b[%d] << %d == %d\n", i, i/8, 7 - (i % 8), bit);

        /* Child D. */
        bit = !!((a[i/8] << i % 8) & 0x80);
        offspringD[i/8] |= bit << (7 - (i % 8));
        //printf("%d a[%d] << %d == %d\n", i, i/8, 7 - (i % 8), bit);
    }
    //printf("\n");

    /* After cross over print C. */
    #if DEBUG
    for (i = 0; i < (length + 7)/8; i++) {
        for (j = 0; j < 8; j++)
            printf("%d", !!((offspringC[i] << j) & 0x80));
    }
    printf("\n");
    #endif

    /* After cross over print D. */
    #if DEBUG
    for (i = 0; i < (length + 7)/8; i++) {
        for (j = 0; j < 8; j++)
            printf("%d", !!((offspringD[i] << j) & 0x80));
    }
    printf("\n\n");
    #endif

    strncpy(childrenBitStrings[index], offspringC, (length + 7)/8);
    added++;

    /* Don't add extra child on odd population size. */
    if (index + 1 != maxPopulation) {
        strncpy(childrenBitStrings[index + 1], offspringD, (length + 7)/8);
        added++;
    }

    free(offspringC);
    free(offspringD);

    return added;
}

void mutation(char * bitString, int length) {
    int position = 0;
    double chance = 0;

    #if DEBUG
    int i = 0, j = 0;
    #endif

    /* Generate chance for mutation between 0 and 1. */
    chance = (double)rand()/(double)(RAND_MAX/1);

    #if DEBUG
    printf("Chance for mutation is: %lf\n", chance);
    #endif

    /* (1/bit string length) probability to perform mutation. */
    if (chance < (double) 1/length) {

        /* Choose a random bit to flip. */
        position = rand() % (length - 1);

        #if DEBUG
        printf("Mutating bit at position %d\n", position);
        #endif

        /* Before mutation. */
        #if DEBUG
        for (i = 0; i < (length + 7)/8; i++) {
            for (j = 0; j < 8; j++)
                printf("%d", !!((bitString[i] << j) & 0x80));
        }
        printf("\n");
        #endif

        /* Toggle bit at position */
        bitString[position/8] ^= 1 << (7 - (position % 8));

        /* After mutation. */
        #if DEBUG
        for (i = 0; i < (length + 7)/8; i++) {
            for (j = 0; j < 8; j++)
                printf("%d", !!((bitString[i] << j) & 0x80));
        }
        printf("\n");
        #endif
    }
}

void simulation(char ** parentBitStrings, int currentPopulation, int maxGenerationNum, int length) {
    int i = 0;
    int generationNum = 0;                  // The current generation
    int childrenPopulation = 0;             // Size of the children population
    int maxFitness = length;                // Desirable fitness value
    int seenFitness = 0;                    // The highest seen fitness value
    int generationFitness = 0;              // The fitness of the generation
    int * populationFitness = NULL;         // The fitness values of the bit strings
    double * weightedFitness = NULL;        // The weighted roulette of the bit strings fitness
    char * parentA = NULL;
    char * parentB = NULL;
    char * temp = NULL;                     // Temporary string to hold contents of parent bit strings.
    char ** childrenBitStrings = NULL;      // The children generation bit strings
    
    #if DEBUG
    int j = 0;
    #endif

    /* Create an initialize arrays and strings. */
    populationFitness = calloc(currentPopulation, sizeof(int));
    weightedFitness = calloc(currentPopulation, sizeof(double));
    parentA = calloc(length, sizeof(char));
    parentB = calloc(length, sizeof(char));
    temp = calloc(length, sizeof(char));

    childrenBitStrings = malloc(sizeof(char *) * currentPopulation);
    for (i = 0; i < currentPopulation; i++)
        childrenBitStrings[i] = calloc((length + 7)/8, sizeof(char));

    /* Terminate once generation number has reahed maximum or solution has been found. */
    while (generationNum < maxGenerationNum && seenFitness != maxFitness) {
    
        for (i = 0; i < currentPopulation; i++)
            memset(childrenBitStrings[i], 0, (length + 7)/8);

        /* Calculate the fitness for each individual in the current generation. */
        for (i = 0; i < currentPopulation; i++) {
            strncpy(temp, parentBitStrings[i], (length + 7)/8);
            populationFitness[i] = setBitsFitness(temp, length);
        }

        /* Sort the parentBitStrings and the populationFitness. */
        bubbleSort(parentBitStrings, populationFitness, currentPopulation);

        /* Highest fitness value seen so far. */
        if (seenFitness < populationFitness[currentPopulation - 1])
            seenFitness = populationFitness[currentPopulation - 1];

        /* Get the accumalated standardized fitness for each individual and generation fitness. */
        generationFitness = accumulatedFitness(weightedFitness, populationFitness, currentPopulation);
        
        if (generationNum == 0) {
            printf("STARTING GENERATION\n");
            printf("Highest seen fitness is: %d\n", seenFitness);
            printf("Number of bits set (1): %d\n\n", generationFitness);
        }

        #if DEBUG
        printBitStrings(parentBitStrings, currentPopulation, length);
        printf("\n");
        #endif

        while (childrenPopulation < currentPopulation) {

            /* Perform selection .*/
            strncpy(parentA, parentBitStrings[selection(weightedFitness, currentPopulation)], (length + 7)/8);

            /* Print parentA */
            #if DEBUG
            printf("Parent A: ");
            for (i = 0; i < (length + 7)/8; i++) {
                for (j = 0; j < 8; j++)
                    printf("%d", !!((parentA[i] << j) & 0x80));
            }
            printf("\n");
            #endif

            strncpy(parentB, parentBitStrings[selection(weightedFitness, currentPopulation)], (length + 7)/8);

            /* Print parentB. */
            #if DEBUG
            printf("Parent B: ");
            for (i = 0; i < (length + 7)/8; i++) {
                for (j = 0; j < 8; j++)
                    printf("%d", !!((parentB[i] << j) & 0x80));
            }
            printf("\n");
            #endif

            /* Perform crossover and mutation. */
            childrenPopulation += crossover(childrenBitStrings, childrenPopulation, currentPopulation, parentA, parentB, length);
        }

        #if DEBUG
        printf("Done making children!\n");
        printBitStrings(childrenBitStrings, childrenPopulation, length);
        printf("\n");
        #endif

        /* Apply mutation to children. */
        for (i = 0; i < childrenPopulation; i++)
            mutation(childrenBitStrings[i], length);

        /* Children become the parents now for the next generation. */
        for (i = 0; i < childrenPopulation; i++)
            strncpy(parentBitStrings[i], childrenBitStrings[i], (length +7)/8);
        
        childrenPopulation = 0;
        generationNum++;

        #if DEBUG
        printf("Parents now are: \n");
        printBitStrings(parentBitStrings, currentPopulation, length);
        #endif
    }

    /* Get the final generation fitness taking mutation into account. */
    generationFitness = 0;
    for (i = 0; i < currentPopulation; i++)
        generationFitness += setBitsFitness(childrenBitStrings[i], length);

    printf("Solution found or maximum generation reached!\n");
    printf("GENERATION %d: \n", generationNum);
    printf("Highest seen fitness is: %d\n", seenFitness);
    printf("Number of bits set (1): %d\n", generationFitness);

    /* Free all the allocated memory. */
    for (i = 0; i < currentPopulation; i++)
        free(childrenBitStrings[i]);

    free(childrenBitStrings);
    free(populationFitness);
    free(weightedFitness);
    free(parentA);
    free(parentB);
    free(temp);
}

void printBitStrings(char ** bitStrings, int generationPopulation, int length) {
    int i = 0, j = 0, k = 0;
    
    /* Go through each string in the array. */
    for (i = 0; i < generationPopulation; i++) {

        /* Go through each character in the string.*/
        for (j = 0; j < (length + 7)/8; j++) {

            /* Left shit by K, mask it to 0x80 (1000 0000) and print 1 or 0 through !!. */
            for (k = 0; k < 8; k++)
                printf("%d", !!((bitStrings[i][j] << k) & 0x80)); 
        }
        printf("\n");
    } 
}