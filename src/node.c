#include "includes/node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_new(unsigned int label) {
    Node *n = malloc(sizeof(*n));
    n->parent = n;
    n->rank = 0;
    n->label = label;
    return n;
}

// Recursively looks for the root node the the tree and returns the root
Node *node_root(Node * restrict a) {
    if (a == a->parent)
        return a;
    return node_root(a->parent);
}

/*
 * Merges node a and b setting the node with the highest
 * rank as the new parent. You MUST called node_root on
 * both nodes before calling this function returns 0 if
 * a is the new root, 1 if b
 */
int node_union(Node * restrict a, Node * restrict b) {
    Node *rootA = node_root(a);
    Node *rootB = node_root(b);

    if (rootA == rootB)
        return -1;
    printf("Union %i and %i\n", rootA->label, rootB->label);

    // Union by rank: attach the shorter tree to the taller one.
    if (rootA->rank > rootB->rank) {
        rootB->parent = rootA;
        return 0;
    } else if (rootA->rank < rootB->rank) {
        rootA->parent = rootB;
        return 1;
    } else {
        rootA->parent = rootB;
        rootB->rank += 1;
        return 1;
    }
}

// We only need this since we are interfacing with rust
void node_free(Node ** restrict a) {
    if (*a != NULL) {
        free(*a);
        *a = NULL;
    } else {
        printf("U stupid\n");
    }
}