#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "memory.h"

/*
 * inserts a new node with the given val after n
 */
void insert(NODE* n, char val) {
    NODE* newNode = NEW(NODE);
    newNode->val = val;
    newNode->skip = 1;
    newNode->rewritten = 0;
    newNode->next = n->next;
    n->next = newNode; // important to set this pointer after having already pointed the new node to n->next
}

/*
 * prints the linked list, one val after the other
 * prints a new line after
 */
void printList(NODE* n) {
    while (n != NULL) {
        printf("%c", n->val);
        n = n->next;
    }
    printf("\n");
}

/*
 * resets the skip and rewritten fields of all nodes in the list to zero
 */
void reset(NODE* n) {
    while (n != NULL) {
        n->skip = 0;
        n->rewritten = 0;
        n = n->next;
    }
}

/*
 * returns the size of the list
 */
int size(NODE* n) {
    if (n == NULL) return 0;
    return 1 + size(n->next);
}
