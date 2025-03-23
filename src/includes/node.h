#ifndef NODE_H
#define NODE_H

typedef struct Node {
    struct Node *parent;
    unsigned int rank;
    unsigned int label;
} Node;

Node *node_new(unsigned int label);
Node *node_root(Node * restrict a);
Node *node_union(Node * restrict a, Node * restrict b);
void node_free(void * restrict a);
#endif