#ifndef CHANNELS_H
#define CHANNELS_H

typedef struct ring_buffer_t ring_buffer_t;

ring_buffer_t *ring_new(int size);
unsigned char *ring_read(ring_buffer_t *__restrict__ self);
void ring_write(ring_buffer_t *__restrict__ self, unsigned char *data);

#endif