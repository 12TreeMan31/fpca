#ifndef CHANNELS_H
#define CHANNELS_H

#define TEST

typedef struct ring_buffer_t ring_buffer_t;
typedef ring_buffer_t ring_consumer;
typedef ring_buffer_t ring_producer;

void ring_new(int size, ring_consumer **rx, ring_producer **tx);
void ring_write(ring_producer *restrict self, unsigned char *data);
unsigned char *ring_read(ring_consumer *restrict self);

#endif