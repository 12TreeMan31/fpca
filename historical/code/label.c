#include "xvec.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BACKGROUND 0
#define FOREGROUND 1

typedef struct Node {
    struct Node *parent;
    xvec *child_labels;
    int rank;
    int label;
} Node;

Node *node_init(int label) {
    Node *n = malloc(sizeof(*n));
    n->parent = n;
    n->child_labels = xvec_new(sizeof(int), 0);
    n->rank = 0;
    n->label = label;
    xvec_push(n->bounds, &n->label);
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
        xvec_join(rootA->bounds, rootB->bounds);
        rootB->parent = rootA;
        return rootB;
    } else if (rootA->rank < rootB->rank) {
        xvec_join(rootB->bounds, rootA->bounds);
        rootA->parent = rootB;
        return rootA;
    } else {
        xvec_join(rootB->bounds, rootA->bounds);
        rootA->parent = rootB;
        rootB->rank += 1;
        return rootA;
    }
}


// void ccl_uf(uint8_t *px, size_t size)
//  Init master structure which contains all of the roots
//  Each index in the structure will corospond to a trees label

//  Init an array of equal height and width as image which will hold
//  the label for a spesific label; this structure should have pointers
//  to indviual indexs from the master root.

/*  Begin looping through px untill size {
        Continue if current px := 0

        Begin check for surrounding pixels ([-1,1], -HEIGHT) and (-1, 0)
        This is because those are the only pixels that should be marked

        If any of these pixels have a label add the current pixel to the first
        label found

        Save any other labels found

        UNION-FIND!!!! all them labels
        Compress (optional) mainly for O(n)
    }
*/

// Connected Component Labeling (Union-Find)
int ccl_uf(uint8_t *px, size_t width, size_t height) {
    const int size = width * height;
    const int dy[] = { -width, -width, -width, 0 };
    const int dx[] = { -1, 0, 1, -1 };

    Node **labels = calloc(size, sizeof(Node *));
    xvec *roots = xvec_new(sizeof(Node *), 0);

    // Makes sure 0 is an invalid label
    xvec_push(roots, NULL);

    for (int i = 0; i < size; i++) {
        if (px[i] == BACKGROUND)
            continue;

        // First create a new node this is this pixel and add it do labels
        Node *current = node_init(i);
        labels[i] = current;
        // minLab is going to be the tree we union to
        Node *minLab = current;
        Node *foundLab[4] = { 0 };

        // Checks neighboring pixels
        for (int j = 0; j < 4; j++) {
            int edgeIdx = i + dy[j] + dx[j];

            // This is just the low handing fruit;
            // We need to handle when pixel is on edge of image
            if (edgeIdx < 0 || edgeIdx > size || labels[edgeIdx] == NULL)
                continue;


            foundLab[j] = find_root(labels[edgeIdx]);
            // For now this can be by index but it really should be by rank
            if (foundLab[j]->rank < minLab->rank)
                minLab = foundLab[j];
        }

        // No other labels
        if (minLab == current) {
            printf("Creating new root at %i\n", i);
            xvec_push(roots, current);
            continue;
        }

        for (int j = 0; j < 4; j++) {
            if (foundLab[j] == minLab || foundLab[j] == NULL)
                continue;

            Node *oldRoot = node_union(minLab, foundLab[j]);
            if (oldRoot == NULL)
                continue;
            for (int k = 0; k < roots->length; k++) {
                if (xvec_get(roots, k) == oldRoot) {
                    xvec_remove(roots, k);
                    break;
                }
            }
        }
    }

    // Print children to output
    for (int i = 0; i < roots->length; i++) {
        Node *tmp = xvec_get(roots, i);
        if (tmp == NULL) {
            continue;
        }
        for (int i = 0; i < tmp->bounds->length; i++) {
            printf("%i, ", *(int *) xvec_get(tmp->bounds, i));
        }
        printf("\n");
    }
    return roots->length - 1;
}


/*  OLD STUFF THATS JUST FOR DOCUMENTATION

#define BPP 8
#define INDEX(x, y) (y * HEIGHT) + x

// direction vectors
static const int dx[] = { -1, 0, 1 };
static const int dy[] = { -HEIGHT, 0, HEIGHT };

int labels[WIDTH][HEIGHT] = { 0 };

// depth first search
void dfs(int x, int y, int current_label, uint8_t *px) {
    if (x < 0 || x == WIDTH)
        return; // out of bounds
    if (y < 0 || y == HEIGHT)
        return; // out of bounds
    if (labels[x][y] || !px[INDEX(x, y)])
        return; // already labeled or not marked with 1 in m

    // mark the current cell
    labels[x][y] = current_label;

    // recursively mark the neighbors
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dfs(x + dx[j], y + dy[i], current_label, px);
        }
    }
}

// connected-component labeling
void cc_label(uint8_t *pixel_data) {
    // Fix this to support bpp
    int total_labels = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (!labels[j][i] && pixel_data[INDEX(j, i)]) {
                total_labels++;
                dfs(j, i, total_labels, pixel_data);
            }
        }
    }
    printf("%i\n", total_labels);
}


// direction vectors
const int dx[] = { +1, 0, -1, 0 };
const int dy[] = { 0, +1, 0, -1 };

// matrix dimensions
int row_count;
int col_count;

// the input matrix
int m[MAX][MAX];

// the labels, 0 means unlabeled
int label[MAX][MAX];

void dfs(int x, int y, int current_label) {
    if (x < 0 || x == row_count)
        return; // out of bounds
    if (y < 0 || y == col_count)
        return; // out of bounds
    if (label[x][y] || !m[x][y])
        return; // already labeled or not marked with 1 in m

    // mark the current cell
    label[x][y] = current_label;

    // recursively mark the neighbors
    for (int direction = 0; direction < 4; ++direction)
        dfs(x + dx[direction], y + dy[direction], current_label);
}

void find_components() {
    int component = 0;
    for (int i = 0; i < row_count; ++i)
        for (int j = 0; j < col_count; ++j)
            if (!label[i][j] && m[i][j])
                dfs(i, j, ++component);
}*/