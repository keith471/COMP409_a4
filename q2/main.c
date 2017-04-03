#include <stdio.h>
#include <stdlib.h>
#include "dfa.h"

long STRINGLENGTH = 100;

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

int main(int argc, char* argv[]) {
    int numThreads;

    if (argc != 2) {
        fprintf(stderr, "USAGE: <program> num_threads\n");
        exit(1);
    }

    char *ptr;
    numThreads = (int) strtol(argv[1], &ptr, 10);

    // generate the string
    char str[STRINGLENGTH];
    genString(str);
    //printString(str);

    // initialize the dfa



}
