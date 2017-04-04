#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "dfa.h"
#include "results.h"

/*
 * We need a function that takes a string, a starting character, and a number of characters
 * and produces a processed string that corresponds to the section of the input string it
 * is responsible for but having blanked out characters that would surely not match as part
 * of a floating point number.
 *
 * input - the input string
 * starting state - the state to start at
 * start - the index of the character to start at
 * numChars - the number of characters to match on
 * lastSection - whether this is the last section of the input string (in which case
 *   numChars can be ignored, and we don't leave any trailing characters)
 */
RESULT* match(char* input, STATE* startingState, int start, int numChars, int lastSection) {

    // the index at which to stop looping
    int end;
    if (lastSection) {
        end = strlen(input) - 1;
    } else {
        end = start + numChars;
    }

    // the blanked-out result string
    char* blankedStr = malloc((end - start + 1) * sizeof(char));
    blankedStr[end - start] = '\0'; // null terminate it
    // copy the appropriate part of the input string over to it
    strncpy(blankedStr, input + start, end - start);

    // the index where we started the last potential match
    int lastStart = 0;

    // the current and previous states
    STATE* prevState = startingState;
    STATE* currState;

    // loop
    int i;
    for (i = start; i < end; i++) {
        // move to the next state using input[i]
        currState = getNextState(prevState, input[i]);
        if (currState == NULL) {
            printf("i is: %d\n", i);
            printf("state is null!\n");
        }
        if (currState->isReject) {
            // if the previous state was accept and input[i] is an 'x', then we found the end of a match
            // else, everything we've been matchin on so far should be blanked out
            if (prevState->isAccept && input[i] == 'x') {
                // blank out just the x
                blank(blankedStr, i - start, i - start);
            } else {
                // blank out everything between lastStart and i
                blank(blankedStr, lastStart, i - start);
            }
            prevState = STARTSTATE;
            lastStart = i - start + 1;
        } else {
            prevState = currState;
        }
    }

    int lastPartialStart;
    if (currState->isReject) {
        lastPartialStart = -1;
    } else {
        lastPartialStart = lastStart;
        if (lastSection) {
            blank(blankedStr, lastPartialStart, strlen(blankedStr));
        }
    }

    // we've iterated through the portion of the input that we were meant to
    // so we can return a result
    RESULT* res = NEW(RESULT);
    res->str = blankedStr;
    res->lastPartialStart = lastPartialStart;
    res->endingState = prevState;
    return res;
}

/*
 * does not aim to be efficient
 */
OPTIMISTICRESULT* optimisticMatch(char* str, int start, int numChars, int lastSection) {
    OPTIMISTICRESULT* res = NEW(OPTIMISTICRESULT);
    res->startState = match(str, STARTSTATE, start, numChars, lastSection);
    res->state1 = match(str, STATE1, start, numChars, lastSection);
    res->state2 = match(str, STATE2, start, numChars, lastSection);
    res->state3 = match(str, STATE3, start, numChars, lastSection);
    res->acceptState = match(str, ACCEPTSTATE, start, numChars, lastSection);
    return res;
}

/*
 * turns all characters in str between start and end, inclusive, to ' '
 */
void blank(char* str, int start, int end) {
    int i;
    for (i = start; i <= end; i++) {
        str[i] = ' ';
    }
}
