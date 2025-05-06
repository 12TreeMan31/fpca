#ifndef CHANNELS_H
#define CHANNELS_H

#include <stdlib.h>

typedef struct ring_buffer ring_buffer;
ring_buffer *ring_new(size_t size);
unsigned char *ring_write(ring_buffer *self, unsigned char *new_data);
unsigned char *ring_read(ring_buffer *self);

#endif