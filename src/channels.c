#include "includes/channels.h"
#include "includes/constants.h"
#include <stdlib.h>

typedef struct ring_buffer {
    unsigned char *buffer[3];
    int head;
    int tail;
} ring_buffer;

static unsigned char *next(ring_buffer *self, int *io_head) {
    unsigned char *next_frame = self->buffer[*io_head];
    *io_head = (*io_head + 1) % 3;

    return next_frame;
}

/* Takes new_data and swaps it out with the buffer `head` is currently pointing to.
   Then moves `head` by one and will move `tail` to the newest pice of data if head catches up*/
unsigned char *ring_write(ring_buffer *self, unsigned char *new_data) {
    // Swaps out old frame with new frame
    unsigned char *next_frame = self->buffer[self->head];
    self->buffer[self->head] = new_data;

    (void *) next(self, &self->head);

    // If write head it the next frame kick it back and index to be on fresh data
    if (self->head == self->tail) {
        if (self->tail == 0) {
            self->tail = 2;
        } else {
            self->tail -= 1;
        }
    }
    return next_frame;
}

unsigned char *ring_read(ring_buffer *self) {
    if (self->head == self->tail)
        // No new data to read
        return NULL;

    return next(self, &self->tail);
}

ring_buffer *ring_new(size_t size) {
    ring_buffer *buf = (ring_buffer *) malloc(sizeof(*buf));
    buf->head = 0;
    buf->tail = 0;
    for (int i = 0; i < 3; i++) {
        buf->buffer[i] = (unsigned char *) malloc(size);
    }
    return buf;
}

#ifdef TEST
#    include <stdio.h>

int main() {
    ring_buffer comms = ring_new(sizeof(char));
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

#endif