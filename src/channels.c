#include "channels.h"
#include "constants.h"

#include <assert.h>
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
static void init(ring_buffer_t *ring, int size) {
    static_assert((RING_SIZE < 2 || (RING_SIZE & RING_MASK)) == 0, "RING_SIZE is not multiple of 2");

    ring->buffer_size = size;
    ring->buffer = calloc(RING_SIZE, ring->buffer_size);
    ring->tail = 0;
    ring->head = 0;
}

// Returns ptr to buffer[*io_head] and moves io_head to next buffer spot
static unsigned char *next(ring_buffer_t *restrict self, int *io_head) {
    unsigned char *next_frame = self->buffer[*io_head];
    *io_head = (*io_head + 1) & RING_MASK;

    return next_frame;
}

static unsigned char *read(ring_buffer_t *restrict self) {
    if (self->head == self->tail)
        // No new data to read
        return NULL;

    return next(self, &self->tail);
}

static void write(ring_buffer_t *restrict self, unsigned char *data) {
    memcpy(self->buffer[self->head], data, self->buffer_size);
    (void *)next(self, &self->head);

    if (self->head == self->tail)
        (void *)next(self, &self->tail);
}

/* Public facing */

typedef ring_buffer_t ring_consumer;
typedef ring_buffer_t ring_producer;

void ring_new(int size, ring_consumer **rx, ring_producer **tx) {
    ring_buffer_t *internal = malloc(sizeof(*internal));
    init(internal, size);

    *rx = (ring_consumer *)internal;
    *tx = (ring_producer *)internal;
}

void ring_write(ring_producer *restrict self, unsigned char *data) {
    write((ring_buffer_t *)self, data);
}

unsigned char *ring_read(ring_consumer *restrict self) {
    return read((ring_buffer_t *)self);
}

/*typedef struct ring_buffer_t {
    unsigned char *buffer[3];
    int write_head;
    int read_head;
} ring_buffer_t;

static unsigned char *next(ring_buffer_t *restrict self, int *io_head) {
    unsigned char *next_frame = self->buffer[*io_head];
    *io_head = (*io_head + 1) % 3;

    return next_frame;
}

static void swap(void *x, void *y) {
}

// We are doing this to make the rust borrow checker happy
typedef struct ring_head {
    const ring_buffer_t *buffer;
} ring_consumer, ring_writer;

void ring_split(ring_buffer_t *restrict buf, ring_consumer *rx, ring_writer *tx) {
    rx->buffer = buf;
    tx->buffer = buf;
}


static unsigned char *swap(ring_buffer_t *self, int *io_head, unsigned char *new_data) {
    unsigned char *current_frame = self->buffer[*io_head];
    self->buffer[*io_head] = new_data;

    return current_frame;
}

Takes new_data and swaps it out with the buffer `head` is currently pointing to.
   Then moves `head` by one and will move `tail` to the newest pice of data if head catches
up unsigned char *ring_write(ring_buffer_t *self, unsigned char *new_data) {
    // Swaps out old frame with new frame
    unsigned char *next_frame = self->buffer[self->write_head];
    self->buffer[self->write_head] = new_data;

    (void *) next(self, &self->write_head);

    // If write head it the next frame kick it back and index to be on fresh data
    if (self->write_head == self->read_head) {
        if (self->read_head == 0) {
            self->read_head = 2;
        } else {
            self->read_head -= 1;
        }
    }
    return next_frame;
}

unsigned char *ring_read(ring_buffer_t *self) {
    if (self->write_head == self->read_head)
        // No new data to read
        return NULL;

    return next(self, &self->read_head);
}

ring_buffer_t *ring_new(unsigned int size) {
    ring_buffer_t *buf = (ring_buffer_t *) malloc(sizeof(*buf));
    buf->write_head = 0;
    buf->read_head = 0;
    for (int i = 0; i < 3; i++) {
        buf->buffer[i] = (unsigned char *) malloc(size);
    }
    return buf;
}

#ifdef TEST
#    include <stdio.h>

int main() {
    ring_buffer_t comms = ring_new(sizeof(char));
    unsigned char *master_ptr;
    for (unsigned char i = 0; i < 255; i++) {
        master_ptr = ring_write(&comms, &i);
        printf("%d, ", i);

        if ((i % 3) == 0 || (i % 5) == 0) {
            unsigned char val = *ring_read(&comms);
            printf("\nVal: %d\n", val);
        }
    }
    fflush(stdout);
}

#endif*/