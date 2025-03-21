#ifndef NODE_H
#define NODE_H

typedef struct Node {
    struct Node *parent;
    int rank;
    int label;
} Node;

Node *node_new(int label);
Node *node_root(Node *a);
Node *node_union(Node *a, Node *b);

#endif