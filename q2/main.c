#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dfa.h"
#include "results.h"

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

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

char* concat(const char *s1, const char *s2) {

    char *result = malloc(strlen(s1)+strlen(s2)+1); // + 1 for the null-terminator
    // should check for malloc errors here
    printf("3-\n");
    strcpy(result, s1);
    printf("4-\n");
    strcat(result, s2);
    printf("5-\n");
    return result;
}

char* combine(RESULT* firstRes, OPTIMISTICRESULT* optiRes[], int numThreads) {

    RESULT* currRes = firstRes;

    // first, blank out anything that needs to be blanked
    int i;
    for (i = 0; i < numThreads; i++) {
        if (currRes->endingState->isStarting) {
            if (currRes->lastPartialStart != -1 && optiRes[i]->startState->str[0] == ' ') {
                blank(currRes->str, currRes->lastPartialStart, strlen(currRes->str) - 1);
            }
            currRes = optiRes[i]->startState;
        } else if (currRes->endingState->isState1) {
            if (currRes->lastPartialStart != -1 && optiRes[i]->state1->str[0] == ' ') {
                blank(currRes->str, currRes->lastPartialStart, strlen(currRes->str) - 1);
            }
            currRes = optiRes[i]->state1;
        } else if (currRes->endingState->isState2) {
            if (currRes->lastPartialStart != -1 && optiRes[i]->state2->str[0] == ' ') {
                blank(currRes->str, currRes->lastPartialStart, strlen(currRes->str) - 1);
            }
            currRes = optiRes[i]->state2;
        } else if (currRes->endingState->isState3) {
            if (currRes->lastPartialStart != -1 && optiRes[i]->state3->str[0] == ' ') {
                blank(currRes->str, currRes->lastPartialStart, strlen(currRes->str) - 1);
            }
            currRes = optiRes[i]->state3;
        } else if (currRes->endingState->isAccept) {
            if (currRes->lastPartialStart != -1 && optiRes[i]->acceptState->str[0] == ' ') {
                blank(currRes->str, currRes->lastPartialStart, strlen(currRes->str) - 1);
            }
            currRes = optiRes[i]->acceptState;
        }
    }

    // now, concatenate all the strings
    currRes = firstRes;
    char* res = firstRes->str;
    for (i = 0; i < numThreads; i++) {
        if (currRes->endingState->isStarting) {
            res = concat(res, optiRes[i]->startState->str);
            currRes = optiRes[i]->startState;
        } else if (currRes->endingState->isState1) {
            res = concat(res, optiRes[i]->state1->str);
            currRes = optiRes[i]->state1;
        } else if (currRes->endingState->isState2) {
            res = concat(res, optiRes[i]->state2->str);
            currRes = optiRes[i]->state2;
        } else if (currRes->endingState->isState3) {
            res = concat(res, optiRes[i]->state3->str);
            currRes = optiRes[i]->state3;
        } else if (currRes->endingState->isAccept) {
            res = concat(res, optiRes[i]->acceptState->str);
            currRes = optiRes[i]->acceptState;
        }
    }

    return res;
}

char* process(char* str, int numThreads) {
    // divide the string into numThreads + 1 pieces
    // first thread gets the first part of the string and performs normal matching on it
    // meanwhile, numThreads get the other parts of the string and each try matching from
    // every state possible
    // if/when all five possibilities converge, the thread may start normal, sequential recognition --> what does this mean?
    // each optimistic thread could keep a map from starting state to resulting string

    // compute the number of sections
    int numSections = min(strlen(str), numThreads + 1);
    // the section size is guaranteed to be at least 1
    int sectionSize = strlen(str) / numSections;
    // the last section could be a little longer but we don't care since match has a flag for this

    // Prepare the results structs
    RESULT* firstSectionRes;

    OPTIMISTICRESULT* optiRes[numSections - 1];
    int i;
    for (i = 0; i < numSections - 1; i++) {
        optiRes[i] = NULL;
    }

    #pragma omp sections
    {
        #pragma omp section
        {
            if (numSections == 1) {
                firstSectionRes = match(str, STARTSTATE, 0, sectionSize, 1);
            } else {
                firstSectionRes = match(str, STARTSTATE, 0, sectionSize, 0);
            }
        }

        #pragma omp section
        {
            #pragma omp parallel for num_threads(numThreads)
                // each thread assigned a section of the string
                // tries matching starting from every state possible
                // saves the result in a map
                for (i = 0; i < numSections - 1; i++) {
                    if (i == numSections - 2) {
                        optiRes[i] = optimisticMatch(str, (i + 1) * sectionSize, sectionSize, 1);
                    } else {
                        optiRes[i] = optimisticMatch(str, (i + 1) * sectionSize, sectionSize, 0);
                    }
                }
        }
    }

    // at this point, all threads have rejoined and we can sequentially handle the entire string, using
    // the ending state of the first section as the starting state of the second, etc.
    // so we need to number the sections of strings

    char* blankedStr;

    if (numSections == 1) {
        blankedStr = firstSectionRes->str;
    } else {
        blankedStr = combine(firstSectionRes, optiRes, numThreads);
    }

    return blankedStr;
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

    // test out match

    RESULT* res = match("839985.39x593809.4x549.3", STATE1, 10, -1, 1);

    printf("%d\n", strlen(res->str));
    printf("%s\n", res->str);

    // process it!
    char* blankedStr = process("839985.39x593809.4x549.3", numThreads);

    printf("%s\n", blankedStr);
}
