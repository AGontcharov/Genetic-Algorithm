# Genetic Algorithm
> Optimizing bit strings through genetics and natural selections

![](https://nnpdf.hepforge.org/images/GA.png)

Implementation of a genetic algorithm written in pure C that optimizes arbitrary bit strings of sizes 8. Given a random starting population of octane bit strings a fitness function which evaluates the number of set bits is applied to determine the fitness value of each bit string (individual). Two candidates are then chosen through [fitness proportionate selection](https://en.wikipedia.org/wiki/Fitness_proportionate_selection) (roulette wheel selection) and single point crossover is performed to produce offsprings from those two candidates. This process is repeated until the children generation matches that of the parent generation. In addition, each offspring will have a **1 / bit string length** chance to be selected for mutation where a random bit will be toggled. The simulation is ran until the maximum generation is reached or a solution based on the fitness function has been found i.e. if the bit strings are of length of 16 then the solution is 16.

## Installation

Linux & OS X:

```sh
git clone https://github.com/AGontcharov/Genetic-Algorithm.git
cd Genetic-Algorithm/
make
```

Windows:

```sh
Not yet available
```

## Running

The Genetric Algorithm accepts a number of arguments that must be supplied to it and are listed in the following order:

|     Argument     |          Description          |
|------------------|-------------------------------|
| arg1             | Number of generations         |
| arg2             | Population size               |
| arg3             | Bit string length             |
| arg4 (optional)  | Randon number generator seed  |

Alternatively, arg1, arg2 and arg3 can be changed inside the Makefile. By default these values have the following:

* Number of Generations = 1000
* Population Size = 100
* Bit string length = 16

Linux & OS X:

```sh
To run via Makefile:
make run

Otherwise, to run it separately:
./runMe [arg1] [arg2] [arg3] [seed]
```

## Usage Example

## Meta

Alexander Gontcharov – alexander.goncharov@gmail.com

[https://github.com/AGontcharov/](https://github.com/AGontcharov/)
