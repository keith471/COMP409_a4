#include <stdio.h>
#include <stdlib.h>
#include "rules.h"
#include "linkedlist.h"
#include "memory.h"

NODE* initLinkedList(int initState) {
    NODE* n = NULL;
    if (initState == 1 || initState == 2 || initState == 3) {
        // initial string: a
        n = NEW(NODE);
        n->val = 'a';
        n->skip = 0;
        n->rewritten = 0;
        n->next = NULL;
    } else if (initState == 4) {
        // initial string: b
        n = NEW(NODE);
        n->val = 'b';
        n->skip = 0;
        n->rewritten = 0;
        n->next = NULL;
    }
    return n;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

//==============================================================================
// Generator
//==============================================================================

void gen(NODE* head, int numThreads, void (*rule1)(NODE*, int), void (*rule2)(NODE*, int)) {
    // need to split the list into sections, one section per thread
    int listSize = size(head);
    int sectionSize = listSize / numThreads;
    if (sectionSize == 0) sectionSize = 1;
    // the last section might be a little longer, by the amount spillOver
    int spillOver = listSize % numThreads;

    int i;
    int count;
    NODE* start = head;
    #pragma omp parallel for num_threads(numThreads)
        for (i = 0; i < min(numThreads, listSize); i++) {
            // iterate to the proper starting node, so we don't need to repeat the iteration for each rule
            start = head;
            count = 0;
            while (count < i * sectionSize && start != NULL) {
                if (!start->skip) {
                    count++;
                }
                start = start->next;
            }
            if (i == numThreads - 1) {
                rule1(start, sectionSize + spillOver);
                rule2(start, sectionSize + spillOver);
            } else {
                rule1(start, sectionSize);
                rule2(start, sectionSize);
            }
        }
}

//==============================================================================
// Runner
//==============================================================================

/*
 * alternates between generation and printing
 */
void run(NODE* head, int numGens, int numThreads, void (*rule1)(NODE*, int), void (*rule2)(NODE*, int)) {
    // first, print the current string
    printList(head);

    // alternate generation and printing for numGens iterations
    int i;
    for (i = 0; i < numGens; i++) {
        gen(head, numThreads, rule1, rule2); // we can do this with multiple threads
        reset(head); // we'll do this with one thread, though we could use more
        printList(head); // we have to do this with one thread
    }
}

//==============================================================================
// Kick-starts the program
//==============================================================================

void start(int initState, int numGens, int numThreads) {
    NODE* head = initLinkedList(initState);
    switch (initState) {
        case 1:
            run(head, numGens, numThreads, a_to_ab, b_to_ba);
            break;
        case 2:
            run(head, numGens, numThreads, a_to_b, b_to_ba);
            break;
        case 3:
            run(head, numGens, numThreads, a_to_aba, b_to_bbb);
            break;
        case 4:
            //run(head, numGens, );
            break;
        default:
            fprintf(stderr, "ERROR: Invalid initial state. Valid options are 1, 2, 3, or 4.\n");
            exit(1);
            break;
    }
}

//==============================================================================
// Main
//==============================================================================

int main(int argc, char* argv[]) {
    int numThreads;
    int numGens;
    int initState;

    if (argc != 4) {
        fprintf(stderr, "USAGE: <program> num_threads num_generations init_state\n");
        exit(1);
    }

    char *ptr;
    numThreads = (int) strtol(argv[1], &ptr, 10);
    numGens = (int) strtol(argv[2], &ptr, 10);
    initState = (int) strtol(argv[3], &ptr, 10);

    start(initState, numGens, numThreads);
}
