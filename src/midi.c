#include "midi.h"

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CLOCK    0xf8
#define MIDI_START    0xfa
#define MIDI_CONTINUE 0xfb
#define MIDI_STOP     0xfc

void handle_voice_msg(midi_t *m, uint8_t byte);
void handle_rt_msg(midi_t *m, uint8_t byte);

void midi_init(midi_t *m)
{
  m->running_status = 0;
  m->expected = 0;
  m->data_idx = 0;
}

void midi_process(midi_t *m, uint8_t data)
{
  if (data >= MIDI_CLOCK)
    handle_rt_msg(m, data);

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
      handle_voice_msg(m, m->running_status);
      m->data_idx = 0;
    }
  }
}

void handle_rt_msg(midi_t *m, uint8_t byte)
{
  switch (byte) {
    case MIDI_CLOCK:
      m->rt_clock(m->callback_data);
      break;
    case MIDI_START:
      m->rt_stop(m->callback_data);
    case MIDI_CONTINUE:
      m->rt_start(m->callback_data);
      break;
    case MIDI_STOP:
      m->rt_stop(m->callback_data);
      break;
  }
}

void handle_voice_msg(midi_t *m, uint8_t byte)
{
  uint8_t *msg = m->data;
  uint8_t status = byte & 0xf0;
  uint8_t channel = byte & 0x0f;

  switch (status) {
    case MIDI_NOTE_ON:
      if (msg[1] == 0)
        m->note_off(m->callback_data, channel, msg[0]);
      else
        m->note_on(m->callback_data, channel, msg[0]);
      break;

    case MIDI_NOTE_OFF:
      m->note_off(m->callback_data, channel, msg[0]);
      break;

    default:
      break;
  }
}

