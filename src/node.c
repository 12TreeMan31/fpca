#include "node.h"

#include <string.h>

Node *node_init(int label) {
    Node *n = malloc(sizeof(*n));
    n->parent = n;
    n->rank = 0;
    n->label = label;
    return n;
}

Node *find_root(Node *a) {
    if (a == a->parent)
        return a;
    return find_root(a->parent);
}

// Return the merged node
Node *node_union(Node *a, Node *b) {
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