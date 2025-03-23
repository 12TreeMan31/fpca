#ifndef XVEC_H
#define XVEC_H

/*
 * xvec.h -- generic dynamic array using x micros
 */

typedef struct xvec {
    unsigned int length;
    unsigned int capacity;
    unsigned int data_size;
    void *data;
} xvec;


xvec *xvec_new(unsigned int data_size, unsigned int capacity);
void xvec_free(xvec *v);
void xvec_resize(xvec *v, unsigned int n_bytes);

void xvec_join(xvec *v1, xvec *v2);
void *xvec_get(xvec *v, unsigned int index);
void xvec_remove(xvec *v, unsigned int index);
void xvec_insert(xvec *v, unsigned int index);
void xvec_push(xvec *v, void *val);
void *vec_pop(xvec *v);
#endif