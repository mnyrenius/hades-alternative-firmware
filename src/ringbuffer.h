#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

#define RINGBUFFER_SIZE 32

struct ringbuffer_t {
  uint8_t buffer[RINGBUFFER_SIZE];
  volatile uint8_t read_pos;
  volatile uint8_t write_pos;
};

void ringbuffer_init(struct ringbuffer_t *rb);
void ringbuffer_write(struct ringbuffer_t *rb, uint8_t data);
int ringbuffer_read(struct ringbuffer_t *rb, uint8_t *data);

#endif /* RINGBUFFER_H */
