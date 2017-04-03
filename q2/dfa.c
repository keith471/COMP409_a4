#include <string.h>
#include "dfa.h"
#include "memory.h"

/*
THE DFA STATES:

        state1

start           state3      accept

        state2
*/

STATE* getTransitionlessSTATE() {
    STATE* s = NEW(STATE);
    s->isAccept = 0;
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
    STATE* startState = getTransitionlessSTATE();
    STATE* state1 = getTransitionlessSTATE();
    STATE* state2 = getTransitionlessSTATE();
    STATE* state3 = getTransitionlessSTATE();
    STATE* acceptState = getTransitionlessSTATE();

    // create transitions for the start state
    TRANSITION* startTransitions = prependTRANSITION(makeTRANSITIONrange("1-9", state2), makeTRANSITION('0', state1));
    // set them on the state
    startState->transitions = startTransitions;

    // create transitions for state 1
    TRANSITION* state1Transition = makeTRANSITION('.', state3);
    // set them on the state
    state1->transitions = state1Transition;

    // create transitions for state 2
    TRANSITION* state2Transitions = prependTRANSITION(makeTRANSITIONrange("0-9", state2), makeTRANSITION('.', state3));
    // set them on the state
    state2->transitions = state2Transitions;

    // create transitions for state 3
    TRANSITION* state3Transitions = makeTRANSITIONrange("0-9", acceptState);
    // set them on the state
    state3->transitions = state3Transitions;

    // create transitions for the accept state
    TRANSITION* acceptTransitions = makeTRANSITIONrange("0-9", acceptState);
    // set them on the state
    acceptState->transitions = acceptTransitions;
}
