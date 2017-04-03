#ifndef DFA_H
#define DFA_H

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
    struct TRANSITION* transitions;
} STATE;


#endif
