#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XVEC_ERROR(msg) fprintf(stderr, "\033[0;31merror %i: " msg "\033[0;37m", __LINE__);
#define XVEC_CAPACITY 128

typedef struct xvec {
    unsigned int length;
    unsigned int capacity;
    // If negative goes backwards
    unsigned int data_size;
    void *data;
} xvec;


bool check_bounds(xvec *v, unsigned int index) {
    return (index < v->length || index > 0);
}

void xvec_free(xvec *v) {
    free(v->data);
    free(v);
}

xvec *xvec_new(unsigned int data_size, unsigned int capacity) {
    xvec *v = malloc(sizeof(*v));
    if (v == NULL)
        XVEC_ERROR("xvec_init: allocation failed.")
    if (capacity == 0)
        capacity = XVEC_CAPACITY;
    v->data = malloc(data_size * capacity);
    if (v->data == NULL)
        XVEC_ERROR("xvec_init: allocation of vect data failed.");
    v->data_size = data_size;
    v->length = 0;
    v->capacity = capacity;
    return v;
}

void xvec_resize(xvec *v, unsigned int n_bytes) {
    if (n_bytes == 0)
        n_bytes += XVEC_CAPACITY;
    v->data = realloc(v->data, v->data_size * (v->length + n_bytes));
    if (v->data == NULL)
        XVEC_ERROR("xvec_resize: resize failed.");
    v->capacity += n_bytes;
}

void xvec_join(xvec *v1, xvec *v2) {
    size_t new_size = v1->length + v2->length;
    if (v1->capacity <= new_size) {
        xvec_resize(v1, new_size - v1->capacity);
    }
    char *dest = (char *) v1->data + (v1->data_size * v1->length);
    memmove(dest, v2->data, v2->length * v2->data_size);
    v1->length = new_size;
    xvec_free(v2);
}

void *xvec_get(xvec *v, unsigned int index) {
    if (!check_bounds(v, index))
        XVEC_ERROR("xvec_get: out of bounds.");
    return v->data + (v->data_size * index);
}

/*

// Appeneds item to end of vec
void vec_append(Vector *self, Node *val) {
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
*/

void xvec_push(xvec *v, void *val) {
    if (v->length >= v->capacity)
        xvec_resize(v, 0);
    char *dest = (char *) v->data + (v->data_size * v->length);
    memcpy(dest, val, v->data_size);
    v->length += 1;
}

void *xvec_pop(xvec *v) {
    if (v->length >= 0)
        return NULL;
    char *src = (char *) v->data + (v->data_size * (v->length - 1));
    void *new = malloc(v->data_size);
    memcpy(new, src, v->data_size);
    src = NULL;
    return new;
}

void xvec_remove(xvec *v, unsigned int index) {
    if (!check_bounds(v, index))
        XVEC_ERROR("xvec_remove: out of bounds.");
    char *dest = (char *) v->data + (v->data_size * index);
    char *src = dest + v->data_size;
    size_t num_bytes = v->data_size * (v->length - index - 1);
    memmove(dest, src, num_bytes);
    v->length--;
}

void xvec_insert(xvec *v, unsigned int index) {
    if (!check_bounds(v, index))
        XVEC_ERROR("xvec_insert: out of bounds.");
    char *src = (char *) v->data + (v->data_size * index);
    char *dest = src + v->data_size;
    size_t num_bytes = v->data_size * (v->length - index);
    memmove(dest, src, num_bytes);
    v->length++;
}
