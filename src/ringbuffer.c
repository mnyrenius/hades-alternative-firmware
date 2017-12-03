#include "ringbuffer.h"

static int _empty(ringbuffer_t *rb)
{
  return (rb->write_pos - rb->read_pos) == 0;
}

static int _full(ringbuffer_t *rb)
{
  return (rb->write_pos - rb->read_pos) == (RINGBUFFER_SIZE - 1);
}

void ringbuffer_init(ringbuffer_t *rb)
{
  rb->read_pos = 0;
  rb->write_pos = 0;
}

void ringbuffer_write(ringbuffer_t *rb, uint8_t data)
{
  if (!_full(rb)) {
    uint8_t w = rb->write_pos;
    rb->buffer[w] = data;
    rb->write_pos = (w + 1) & (RINGBUFFER_SIZE - 1);
  }
}

int ringbuffer_read(ringbuffer_t *rb, uint8_t * data)
{
  int ret = -1;

  if (!_empty(rb)) {
    uint8_t r = rb->read_pos;
    *data = rb->buffer[r];
    rb->read_pos = (r + 1) & (RINGBUFFER_SIZE - 1);
    ret = 0;
  }

  return ret;
}

