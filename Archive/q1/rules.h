#ifndef RULES_H
#define RULES_H

#include "linkedlist.h"

void a_to_b(NODE* n, int startAt, int numNodes);
void a_to_ab(NODE* n, int startAt, int numNodes);
void a_to_aba(NODE* n, int startAt, int numNodes);
void a_to_ac(NODE* n, int startAt, int numNodes);

void b_to_ba(NODE* n, int startAt, int numNodes);
void b_to_bbb(NODE* n, int startAt, int numNodes);
void b_to_abc(NODE* n, int startAt, int numNodes);

void c_to_ca(NODE* n, int startAt, int numNodes);

#endif
