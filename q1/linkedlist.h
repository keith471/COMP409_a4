#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct NODE {
    char val;
    int skip;   // whether this is a new node added during the current generation
                // and should thus be skipped over (not counted and not regenerated)
    int rewritten; // whether the node has had its value rewritten. If so, then we
                   // count the node but do not regenerate it
    struct NODE* next;
} NODE;

void insert(NODE* n, char val);
void printList(NODE* n);
void reset(NODE* n);
int size(NODE* n);

#endif
