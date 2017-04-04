#ifndef RESULTS_H
#define RESULTS_H

#include "dfa.h"

/*
 * Result for a thread executing on a portion of the string
 * The thread could have started at any state (except the reject state)
 */
typedef struct RESULT {
    char* str; // the modified string
    int lastPartialStart; // the index of the last unmatched sequence
    struct STATE* endingState; // the state ended in
} RESULT;

/*
 * Results for an optimistic thread
 */
typedef struct OPTIMISTICRESULT {
    struct RESULT* startState; // the result if starting in the start state
    struct RESULT* state1; // the result if starting in state 1
    struct RESULT* state2; // ...
    struct RESULT* state3;
    struct RESULT* acceptState;
} OPTIMISTICRESULT;

RESULT* match(char* input, STATE* startingState, int start, int numChars, int lastSection);
OPTIMISTICRESULT* optimisticMatch(char* str, int start, int numChars, int lastSection);
void blank(char* str, int start, int end);

#endif
