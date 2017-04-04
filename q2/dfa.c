#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "memory.h"

/*
THE DFA STATES:

        state1

start           state3      accept      reject

        state2
*/

STATE* getTransitionlessSTATE() {
    STATE* s = NEW(STATE);
    s->isAccept = 0;
    s->isReject = 0;
    s->isState1 = 0;
    s->isState2 = 0;
    s->isState3 = 0;
    s->isStarting = 0;
    s->transitions = NULL;
    return s;
}

STATE* getTransitionlessAcceptSTATE() {
    STATE* s = NEW(STATE);
    s->isAccept = 1;
    s->isReject = 0;
    s->isState1 = 0;
    s->isState2 = 0;
    s->isState3 = 0;
    s->isStarting = 0;
    s->transitions = NULL;
    return s;
}

STATE* getTransitionlessRejectSTATE() {
    STATE* s = NEW(STATE);
    s->isAccept = 0;
    s->isReject = 1;
    s->isState1 = 0;
    s->isState2 = 0;
    s->isState3 = 0;
    s->isStarting = 0;
    s->transitions = NULL;
    return s;
}

TRANSITION* makeTRANSITION(char symbol, STATE* state) {
    TRANSITION* t = NEW(TRANSITION);
    t->symbol = symbol;
    t->state = state;
    t->next = NULL;
    return t;
}

TRANSITION* makeTRANSITIONrange(char* range, STATE* state) {
    TRANSITION* t = NULL;

    if (strcmp(range, "0-9") == 0) {
        t = makeTRANSITION('0', state);
        range = "1-9";
    }

    if (strcmp(range, "1-9") == 0) {
        t = prependTRANSITION(t, makeTRANSITION('1', state));
        t = prependTRANSITION(t, makeTRANSITION('2', state));
        t = prependTRANSITION(t, makeTRANSITION('3', state));
        t = prependTRANSITION(t, makeTRANSITION('4', state));
        t = prependTRANSITION(t, makeTRANSITION('5', state));
        t = prependTRANSITION(t, makeTRANSITION('6', state));
        t = prependTRANSITION(t, makeTRANSITION('7', state));
        t = prependTRANSITION(t, makeTRANSITION('8', state));
        t = prependTRANSITION(t, makeTRANSITION('9', state));
        return t;
    } else {
        fprintf(stderr, "ERROR: invalid range\n");
        exit(1);
    }
    // will never hit this
    return NULL;
}

TRANSITION* prependTRANSITION(TRANSITION* prevs, TRANSITION* curr) {
    curr->next = prevs;
    return curr;
}

void initDfa() {
    // create the states, minus transitions
    STARTSTATE = getTransitionlessSTATE();
    STARTSTATE->isStarting = 1;
    STATE1 = getTransitionlessSTATE();
    STATE1->isState1;
    STATE2 = getTransitionlessSTATE();
    STATE2->isState2;
    STATE3 = getTransitionlessSTATE();
    STATE3->isState3;
    ACCEPTSTATE = getTransitionlessAcceptSTATE();
    REJECTSTATE = getTransitionlessRejectSTATE();

    // create transitions for the start state
    TRANSITION* startTransitions = prependTRANSITION(makeTRANSITIONrange("1-9", STATE2), makeTRANSITION('0', STATE1));
    startTransitions = prependTRANSITION(startTransitions, makeTRANSITION('.', REJECTSTATE));
    startTransitions = prependTRANSITION(startTransitions, makeTRANSITION('x', REJECTSTATE));
    // set them on the state
    STARTSTATE->transitions = startTransitions;

    // create transitions for state 1
    TRANSITION* state1Transitions = prependTRANSITION(makeTRANSITIONrange("0-9", REJECTSTATE), makeTRANSITION('.', STATE3));
    state1Transitions =  prependTRANSITION(state1Transitions, makeTRANSITION('x', REJECTSTATE));
    // set them on the state
    STATE1->transitions = state1Transitions;

    // create transitions for state 2
    TRANSITION* state2Transitions = prependTRANSITION(makeTRANSITIONrange("0-9", STATE2), makeTRANSITION('.', STATE3));
    state2Transitions = prependTRANSITION(state2Transitions, makeTRANSITION('x', REJECTSTATE));
    // set them on the state
    STATE2->transitions = state2Transitions;

    // create transitions for state 3
    TRANSITION* state3Transitions = makeTRANSITIONrange("0-9", ACCEPTSTATE);
    state3Transitions = prependTRANSITION(state3Transitions, makeTRANSITION('.', REJECTSTATE));
    state3Transitions = prependTRANSITION(state3Transitions, makeTRANSITION('x', REJECTSTATE));
    // set them on the state
    STATE3->transitions = state3Transitions;

    // create transitions for the accept state
    TRANSITION* acceptTransitions = makeTRANSITIONrange("0-9", ACCEPTSTATE);
    acceptTransitions = prependTRANSITION(acceptTransitions, makeTRANSITION('.', REJECTSTATE));
    acceptTransitions = prependTRANSITION(acceptTransitions, makeTRANSITION('x', REJECTSTATE));
    // set them on the state
    ACCEPTSTATE->transitions = acceptTransitions;
}

/*
 * returns the next state from currState and using c
 */
STATE* getNextState(STATE* currState, char c) {
    // iterate through currState's transitions for the transition that uses c
    TRANSITION* t = currState->transitions;
    while (t != NULL) {
        if (t->symbol == c) {
            return t->state;
        }
        t = t->next;
    }
    printf("char is: %c\n");
    // will never hit this
    return NULL;
}

/*
 * returns true if the DFA accepts the input when starting from the given state, and false otherwise
 */
int accepts(STATE* currState, char* input) {
    // error cases
    if (currState == NULL) return 0;
    if (currState->isReject) return 0;

    int i;
    for (i = 0; i < strlen(input); i++) {
        // get the next state
        currState = getNextState(currState, input[i]);
        if (currState->isReject) {
            return 0;
        }
    }

    // are we at the accept state?
    if (currState->isAccept) {
        return 1;
    }
    return 0;
}
