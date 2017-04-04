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

void gen(NODE* head, int numThreads, void (*rule1)(NODE*, int, int), void (*rule2)(NODE*, int, int), void (*rule3)(NODE*, int, int)) {
    // need to split the list into sections, one section per thread
    int listSize = size(head);
    int sectionSize = listSize / numThreads;
    int spillOver = 0;
    if (sectionSize == 0) {
        sectionSize = 1;
    } else {
        // the last section might be a little longer, by the amount spillOver
        spillOver = listSize % numThreads;
    }

    int i;
    int count;
    NODE* start = head;
    int iters = min(numThreads, listSize);
    #pragma omp parallel for num_threads(numThreads)
        for (i = 0; i < iters; i++) {
            if (i == (iters - 1)) {
                // always have at least two rules, and maybe a third
                rule1(start, i * sectionSize, sectionSize + spillOver);
                rule2(start, i * sectionSize, sectionSize + spillOver);
                if (rule3 != NULL) {
                    rule3(start, i * sectionSize, sectionSize + spillOver);
                }
            } else {
                rule1(start, i * sectionSize, sectionSize);
                rule2(start, i * sectionSize, sectionSize);
                if (rule3 != NULL) {
                    rule3(start, i * sectionSize, sectionSize);
                }
            }
        }
}

//==============================================================================
// Runner
//==============================================================================

/*
 * alternates between generation and printing
 */
void run(NODE* head, int numGens, int numThreads, void (*rule1)(NODE*, int, int), void (*rule2)(NODE*, int, int), void (*rule3)(NODE*, int, int)) {
    // first, print the current string
    printList(head);

    // alternate generation and printing for numGens iterations
    int i;
    for (i = 0; i < numGens; i++) {
        gen(head, numThreads, rule1, rule2, rule3); // we can do this with multiple threads
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
            run(head, numGens, numThreads, a_to_ab, b_to_ba, NULL);
            break;
        case 2:
            run(head, numGens, numThreads, a_to_b, b_to_ba, NULL);
            break;
        case 3:
            run(head, numGens, numThreads, a_to_aba, b_to_bbb, NULL);
            break;
        case 4:
            run(head, numGens, numThreads, a_to_ac, b_to_abc, c_to_ca);
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
