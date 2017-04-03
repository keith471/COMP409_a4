#include <stdio.h>
#include <stdlib.h>
#include "rules.h"

void a_to_b(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'a') {
                    n->val = 'b';
                    n->rewritten = 1;
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}

void a_to_ab(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'a') {
                    // insert a node after with value 'b'
                    insert(n, 'b');
                }
            }
            count++;
        }
        n = n->next;
    }
}

void a_to_aba(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'a') {
                    // insert 'a' then 'b'
                    insert(n, 'a');
                    insert(n, 'b');
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}

void a_to_ac(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'a') {
                    // insert 'c'
                    insert(n, 'c');
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}

void b_to_ba(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'b') {
                    // insert a node after with value 'a'
                    insert(n, 'a');
                }
            }
            count++;
        }
        n = n->next;
    }
}

void b_to_bbb(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'b') {
                    // insert two b's
                    insert(n, 'b');
                    insert(n, 'b');
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}

void b_to_abc(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'b') {
                    // change the val of this node to 'a'
                    n->rewritten = 1;
                    n->val = 'a';
                    // insert 'c' then 'b'
                    insert(n, 'c');
                    insert(n, 'b'); // don't skip!
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}

void c_to_ca(NODE* n, int numNodes) {
    int count = 0;
    while (count < numNodes && n != NULL) {
        if (!n->skip) {
            if (!n->rewritten) {
                if (n->val == 'c') {
                    // insert 'a'
                    insert(n, 'a');
                }
                count++;
            } else {
                // count it
                count++;
            }
        }
        n = n->next;
    }
}
