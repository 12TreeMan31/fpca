#ifndef VECT_H
#define VECT_H

/*
 * gist: https://gist.github.com/sixthgear/1507970
 * vect.h -- type-safe generic dynamic array
 * made by sixthgear. BSD Licenced.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECT_CAPACITY 128

/* vector type for use in type-unsafe functions */
/* macros will generate type-safe functions for each type called in */
/* VECT_GENERATE_TYPE */

typedef struct vect {
    int length;
    int capacity;
    size_t data_size;
    void *data;
} vect;

void vect_err(const char *error) {
    fprintf(stderr, "%s\n", error);
    exit(1);
}

vect *vect_init(size_t data_size, unsigned int capacity) {
    vect *v = (vect *) malloc(sizeof(vect));
    if (v == NULL)
        vect_err("vect_init: allocation of vect failed.");
    if (capacity == 0)
        capacity = VECT_CAPACITY;
    v->data = malloc(data_size * capacity);
    if (v->data == NULL)
        vect_err("vect_init: allocation of vect data failed.");
    v->data_size = data_size;
    v->length = 0;
    v->capacity = capacity;
    return v;
}

void vect_free(void *v) {
    free(((vect *) v)->data);
    free(((vect *) v));
}

void vect_resize(vect *v, unsigned int n_bytes) {
    if (n_bytes == 0)
        n_bytes += VECT_CAPACITY;
    v->data = realloc(v->data, v->data_size * (v->length + n_bytes));
    if (v->data == NULL)
        vect_err("vect_resize: resize failed.");
    v->capacity += n_bytes;
}

void vect_join(vect *v1, vect *v2) {
    size_t new_size = v1->length + v2->length;
    if (v1->capacity < new_size) {
        vect_resize(v1, new_size - v1->capacity);
    }
    char *dest = (char *) v1->data + (v1->data_size * v1->length);
    memmove(dest, v2->data, v2->length * v2->data_size);
    v1->length = new_size;
    vect_free(v2);
}

unsigned int vect_chk_bounds(vect *v, unsigned int pos) {
    return (pos < v->length);
}

unsigned int vect_at(vect *v, unsigned int pos) {
    if (!vect_chk_bounds((vect *) v, pos))
        vect_err("vect_at: out of bounds.");
    return pos;
}

unsigned int vect_set(vect *v, unsigned int pos) {
    if (!vect_chk_bounds((vect *) v, pos))
        vect_err("vect_set: out of bounds.");
    return pos;
}

unsigned int vect_push(vect *v) {
    if (v->length == v->capacity)
        vect_resize(v, 0);
    return v->length++;
}

unsigned int vect_pop(vect *v) {
    if (v->length == 0)
        vect_err("vect_pop: underflow.");
    return --v->length;
}

unsigned int vect_rem(vect *v, unsigned int pos) {
    if (!vect_chk_bounds((vect *) v, pos))
        vect_err("vect_rem: out of bounds.");
    char *dest = (char *) v->data + (v->data_size * pos);
    char *src = dest + v->data_size;
    size_t num_bytes = v->data_size * (v->length - pos - 1);
    memmove(dest, src, num_bytes);
    v->length--;
    return pos;
}

unsigned int vect_ins(vect *v, unsigned int pos) {
    if (!vect_chk_bounds((vect *) v, pos))
        vect_err("vect_rem: out of bounds.");
    char *src = (char *) v->data + (v->data_size * pos);
    char *dest = src + v->data_size;
    size_t num_bytes = v->data_size * (v->length - pos);
    memmove(dest, src, num_bytes);
    v->length++;
    return pos;
}


/* vector type for use in type-safe functions */
/* functions for each type must be generated by VECT_GENERATE_TYPE(type) */
/* or VECT_GENERATE_NAME(type, name) for pointer types or multiple qualifiers */

#define VECT_GENERATE_TYPE(TYPE) VECT_GENERATE_NAME(TYPE, TYPE)
#define VECT_GENERATE_NAME(TYPE, NAME)                                   \
    typedef struct vect_##NAME {                                         \
        int length;                                                      \
        int capacity;                                                    \
        size_t data_size;                                                \
        TYPE *data;                                                      \
    } vect_##NAME;                                                       \
    /*init vect*/                                                        \
    vect_##NAME *vect_init_##NAME(unsigned int capacity) {               \
        return (vect_##NAME *) vect_init(sizeof(TYPE), capacity);        \
    }                                                                    \
    /*returns ptr of data at pos*/                                       \
    TYPE *vect_ptr_##NAME(vect_##NAME *v, unsigned int pos) {            \
        return v->data + vect_at((vect *) v, pos);                       \
    }                                                                    \
    /*gets value at pos*/                                                \
    TYPE vect_at_##NAME(vect_##NAME *v, unsigned int pos) {              \
        return v->data[vect_at((vect *) v, pos)];                        \
    }                                                                    \
    /*pushes value to vect*/                                             \
    void vect_push_##NAME(vect_##NAME *v, TYPE value) {                  \
        v->data[vect_push((vect *) v)] = value;                          \
    }                                                                    \
    /*set value at pos*/                                                 \
    void vect_set_##NAME(vect_##NAME *v, unsigned int pos, TYPE value) { \
        v->data[vect_set((vect *) v, pos)] = value;                      \
    }                                                                    \
    /*pops value*/                                                       \
    TYPE vect_pop_##NAME(vect_##NAME *v) {                               \
        return v->data[vect_pop((vect *) v)];                            \
    }                                                                    \
    /**/                                                                 \
    void vect_rem_##NAME(vect_##NAME *v, unsigned int pos) {             \
        vect_rem((vect *) v, pos);                                       \
    }                                                                    \
    /*insert val at pos*/                                                \
    void vect_ins_##NAME(vect_##NAME *v, unsigned int pos, TYPE value) { \
        v->data[vect_ins((vect *) v, pos)] = value;                      \
    }                                                                    \
    void vect_app_##NAME(vect_##NAME *v, TYPE value) {                   \
        v->data[vect_ins((vect *) v, v->length)] = value;                \
    }                                                                    \
    /*join two vectors*/                                                 \
    void vect_join_##NAME(vect_##NAME *v1, vect_##NAME *v2) {            \
        vect_join((vect *) v1, (vect *) v2);                             \
    }

#endif