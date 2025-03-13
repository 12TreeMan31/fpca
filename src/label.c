#include "vect.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLACK_IDX 1
#define WHITE_IDX 0

typedef struct Node {
    struct Node *parent;
    int label;
} Node;

Node *node_init(int label) {
    Node *new = malloc(sizeof(*new));
    new->label = label;
    new->parent = new;
    return new;
}

// Recursively look for the parent node
Node *find_root(Node *a) {
    if (a == a->parent)
        return a;
    return find_root(a->parent);
}

// Union the two nodes and return the old root
Node *node_union(Node *a, Node *b) {
    Node *rootA = find_root(a);
    Node *rootB = find_root(b);

    if (rootA == rootB)
        return NULL;

    rootB->parent = rootA;
    return rootB;
}

typedef struct NodeVector {
    size_t length;
    size_t capacity;
    // This will hurt the type system but you can't index into void *
    Node **data;
} Vector;

Vector *vec_init(size_t capacity) {
    Vector *vec = malloc(sizeof(*vec));
    vec->data = calloc(capacity, sizeof(*vec->data));
    vec->capacity = capacity;
    vec->length = 0;
    return vec;
}

// void vec_join(Vector *a, Vector *b);

// Appeneds item to end of vec
void *vec_append(Vector *self, Node *val) {
    self->length += 1;
    if (self->length >= self->capacity) {
        self->capacity += 512;
        self->data = realloc(self->data, sizeof(*self->data) * self->capacity);
        // Zeroizes the new area
        for (int i = self->length; i < self->capacity; i++) {
            self->data[i] = 0;
        }
    }
    self->data[self->length - 1] = val;
}

// Removes value at index and compacts values
void vec_remove(Vector *self, size_t index) {
    for (int i = index; i < self->length - 1; i++) {
        self->data[i] = self->data[i + 1];
    }
    self->length -= 1;
}

// returns value at index
void *vec_get(Vector *self, size_t index) {
    if (index >= self->length) {
        return NULL;
    }
    return self->data[index];
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
void ccl_uf(uint8_t *px, size_t width, size_t height) {
    const int size = width * height;
    const int dy[] = { -width, -width, -width, 0 };
    const int dx[] = { -1, 0, 1, -1 };

    Node **labels = calloc(size, sizeof(Node *));
    Vector *roots = vec_init(1024);

    // Makes sure 0 is an invalid label
    vec_append(roots, NULL);

    for (int i = 0; i < size; i++) {
        if (px[i] == BLACK_IDX)
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
            if (edgeIdx < 0 || edgeIdx > size || px[edgeIdx] == WHITE_IDX)
                continue;

            foundLab[j] = find_root(labels[edgeIdx]);
            // For now this can be by index but it really should be by rank
            if (foundLab[j]->label < minLab->label)
                minLab = foundLab[j];
        }

        // No other labels
        if (minLab == current) {
            printf("Creating new root at %i\n", i);
            vec_append(roots, current);
            continue;
        }

        for (int j = 0; j < 4; j++) {
            if (foundLab[j] == minLab || foundLab[j] == NULL)
                continue;

            Node *oldRoot = node_union(minLab, foundLab[j]);
            if (oldRoot == NULL)
                continue;
            for (int k = 0; k < roots->length; k++) {
                if (vec_get(roots, k) == oldRoot) {
                    vec_remove(roots, k);
                    break;
                }
            }
        }
    }

    printf("----------\n");
    // Print children to output
    for (int i = 0; i < roots->length; i++) {
        Node *tmp = vec_get(roots, i);
        if (tmp == NULL) {
            continue;
        }
    }
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