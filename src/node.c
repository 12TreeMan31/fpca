#include "includes/node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_init(unsigned int label) {
    Node *n = malloc(sizeof(*n));
    n->parent = n;
    n->rank = 0;
    n->label = label;
    return n;
}


// Recursively looks for the root node the the tree and returns the root
Node *find_root(Node * restrict a) {
    if (a == a->parent)
        return a;
    return find_root(a->parent);
}

/*
 * Merges node a and b setting the node with the highest
 * rank as the new parent. You MUST called find_root on
 * both nodes before calling this function
 */
Node *node_union(Node * restrict a, Node * restrict b) {
    Node *rootA = find_root(a);
    Node *rootB = find_root(b);

    if (rootA == rootB)
        return NULL;
    printf("Union %i and %i\n", rootA->label, rootB->label);

    // Union by rank: attach the shorter tree to the taller one.
    if (rootA->rank > rootB->rank) {
        rootB->parent = rootA;
        return rootB;
    } else if (rootA->rank < rootB->rank) {
        rootA->parent = rootB;
        return rootA;
    } else {
        rootA->parent = rootB;
        rootB->rank += 1;
        return rootA;
    }
}

// We only need this since we are interfacing with rust
void node_free(Node * restrict a) {
    free(a);
}