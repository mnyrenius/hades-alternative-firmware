#include "midi.h"
#include <stdint.h>

void _handle_msg(struct midi_t *m, uint8_t byte);

void midi_init(struct midi_t *m)
{
  m->running_status = 0;
  m->expected = 0;
  m->data_idx = 0;
}

void midi_process(struct midi_t *m, uint8_t data)
{
  if (data == 0xfe)
    return;

  if (data >= 0xf8)
    _handle_msg(m, data);

  else {
    if (data >= 0x80) {
      m->running_status = data;

      switch (data & 0xf0) {
        case 0x80:
        case 0x90:
        case 0xe0:
          m->expected = 2;
          m->data_idx = 0;
          break;

        default:
          break;
      }
    } else
      m->data[m->data_idx++] = data;

    if (m->data_idx == m->expected) {
      _handle_msg(m, m->running_status);
      m->data_idx = 0;
    }
  }
}

void _handle_msg(struct midi_t *m, uint8_t byte)
{
  uint8_t *msg = m->data;

  switch (byte & 0xf0) {
    case 0x90:
      if (msg[1] == 0)
        m->note_off(byte & 0x0f);
      else
        m->note_on(byte & 0x0f, msg[0]);
      break;

    case 0x80:
      m->note_off(byte & 0x0f);
      break;

    default:
      break;
  }
}

