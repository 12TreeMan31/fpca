#include "channels.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#define RING_SIZE 4
#define RING_MASK (RING_SIZE - 1)

typedef struct ring_buffer_t {
    int buffer_size;
    unsigned char **buffer;
    int head; // Next fame to use
    int tail; // ditto
} ring_buffer_t;

// Initallizes ring on stack and allocated `RING_SIZE` elements on `buffer`
// TODO: Make relaxed atomic?
ring_buffer_t *ring_new(int size) {
    static_assert((RING_SIZE < 2 || (RING_SIZE & RING_MASK)) == 0, "RING_SIZE is not multiple of 2"); // C11
    ring_buffer_t *ring = (ring_buffer_t *)malloc(sizeof(*ring));

    ring->buffer_size = size;
    ring->tail = 0;
    ring->head = 0;

    // ring->buffer = (unsigned char *)malloc(sizeof(unsigned char *) * RING_SIZE);
    for (int i = 0; i < RING_SIZE; ++i) {
        ring->buffer[i] = (unsigned char *)malloc(size);
    }
    return ring;
}

// Returns ptr to buffer[*io_head] and moves io_head to next buffer spot
static unsigned char *next(ring_buffer_t *__restrict__ self, int *io_head) {
    unsigned char *next_frame = self->buffer[*io_head];
    *io_head = (*io_head + 1) & RING_MASK;

    return next_frame;
}

unsigned char *ring_read(ring_buffer_t *__restrict__ self) {
    if (self->head == self->tail)
        // No new data to read
        return NULL;

    return next(self, &self->tail);
}

void ring_write(ring_buffer_t *__restrict__ self, unsigned char *data) {
    memcpy(self->buffer[self->head], data, self->buffer_size);
    // We call next for its side effect
    next(self, &self->head);

    if (self->head == self->tail)
        next(self, &self->tail);
}

/* Public facing */

/*struct ring_consumer {
    ring_buffer_t *inner;
};

struct ring_producer {
    ring_buffer_t *inner;
};

void ring_new(int size, struct ring_consumer *rx, struct ring_producer *tx) {
    ring_buffer_t *internal = malloc(sizeof(*internal));
    init(internal, size);

    rx->inner = internal;
    tx->inner = internal;
}

void ring_write(struct ring_producer *self, unsigned char *data) {
    write(self->inner, data);
}

unsigned char *ring_read(struct ring_consumer *self) {
    return read(self->inner);
}

#ifdef TEST
#    include <stdio.h>

int main() {
    struct ring_consumer reader;
    struct ring_producer writer;

    ring_new(sizeof(char), &reader, &writer);
    for (unsigned char i = 0; i < 255; i++) {
        ring_write(&writer, &i);
        printf("%d, ", i);

        if ((i % 3) == 0 || (i % 5) == 0) {
            unsigned char val = *ring_read(&reader);
            printf("\nVal: %d\n", val);
        }
    }
    fflush(stdout);
}

#endif*/