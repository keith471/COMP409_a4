#ifndef DFA_H
#define DFA_H

/*
 * defines a transition to another state
 */
typedef struct TRANSITION {
    char symbol;
    struct STATE* state;
    struct TRANSITION* next;
} TRANSITION;

/*
 * each state needs to know whether it is an accept state and it needs to know
 * the transitions that are posible from it
 */
typedef struct STATE {
    int isAccept;
    int isReject;
    int isState1;
    int isState2;
    int isState3;
    int isStarting;
    struct TRANSITION* transitions;
} STATE;

// The DFA states
STATE* STARTSTATE;
STATE* STATE1;
STATE* STATE2;
STATE* STATE3;
STATE* ACCEPTSTATE;
STATE* REJECTSTATE;

STATE* getTransitionlessSTATE();
STATE* getTransitionlessAcceptSTATE();
STATE* getTransitionlessRejectSTATE();
TRANSITION* makeTRANSITION(char symbol, STATE* state);
TRANSITION* makeTRANSITIONrange(char* range, STATE* state);
TRANSITION* prependTRANSITION(TRANSITION* prevs, TRANSITION* curr);
void initDfa();
STATE* getNextState(STATE* currState, char c);
int accepts(STATE* currState, char* input);

#endif
