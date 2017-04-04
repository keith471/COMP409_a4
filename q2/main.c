#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dfa.h"

long STRINGLENGTH = 5;

char CHARACTERS[12] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', 'x'};

void printString(char* str) {
    int i;
    for (i = 0; i < STRINGLENGTH; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
}

void genString(char* str) {
    int i, r;
    for (i =0; i < STRINGLENGTH; i++) {
        r = rand() % 12;
        str[i] = CHARACTERS[r];
    }
}

void process(char* str, int numThreads) {
    // divide the string into numThreads + 1 pieces
    // first thread gets the first part of the string and performs normal matching on it
    // meanwhile, numThreads get the other parts of the string and each try matching from
    // every state possible
    // if/when all five possibilities converge, the thread may start normal, sequential recognition --> what does this mean?
    // each optimistic thread could keep a map from starting state to resulting string

    #pragma omp sections
    {
        #pragma omp section
        {
            printf("I'm the main thread!\n");
        }

        #pragma omp section
        {
            int i;
            #pragma omp parallel for num_threads(numThreads)
                for (i = 0; i < 10; i++) {
                    printf("%d\n", i);
                }

        }
    }

    printf("This is after the sections\n");

    // at this point, all threads have rejoined and we can sequentially handle the entire string, using
    // the ending state of the first section as the starting state of the second, etc.
}

int main(int argc, char* argv[]) {
    int numThreads;

    if (argc != 2) {
        fprintf(stderr, "USAGE: <program> num_threads\n");
        exit(1);
    }

    char *ptr;
    numThreads = (int) strtol(argv[1], &ptr, 10);

    // seed random number generator
    srand(time(NULL));

    // generate the string
    char str[STRINGLENGTH];
    genString(str);

    // print it out, prior to any processing
    printString(str);

    // initialize the dfa
    initDfa();

    // process it!
    process(str, numThreads);
}
